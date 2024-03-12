<!-- vim: set spelllang=de noexpandtab: -->
# Das Problem mit `assert()`

Auf [www.knp.de](https://www.knp.de) halte ich meine Gedanken zu
Programmiersprachen fest. Hauptsächlich werden das erstmal C++ und Oberon
sein. Mit diesen Sprachen befasse ich mich im Augenblick am intensivsten.
Das Ziel ist es, kleine Handreichungen zu geben, um stabilere und bessere
Programme zu entwickeln. Gerade C++ ist eine riesige Sprache und hat einen
sehr starken Fokus auf Geschwindigkeit. Dafür werden oft Sicherheit und
Stabilität geopfert. Auf einer schwankenden Plattform kann aber nur sehr
schwierig etwas Stabiles errichtet werden.

Daher ist der Sinn dieser Beiträge, das Fundament zu festigen. Damit können 
wir leichter gute Programme schreiben. Wenn sie dann etwas langsamer Laufen,
ist das ein geringer Preis für die resultierende Qualitätssteigerung.

Wenn ihr Fehler und Verbesserungen findet, eröffnet bitte Issues auf GitHub
oder sendet mir Pull-Requests.

## Wann sollte ich `assert()` verwenden

Das Makro `assert` ist in C++ ein Überbleibsel aus den C Tagen. Ich verwende
ein kleines Programm als durchgehendes Beispiel: die Ermittlung der Länge
einer Zeichenfolge. Hier zuerst die Implementierung in der Datei `strlen.cpp`:

```c++
#include <cassert>

#include "strlen.h"

[[nodiscard]] size_t strlen(const char* str) {
	assert(str);
	auto cur { str };
	for (; *cur; ++cur) { }
	return cur - str;
}
```

Mein Dank an Sebastian, für die Optimierung dieser Funktion. Ich hatte sie
zuerst mit einem Zähler ohne Zeiger-Arithmetik implementiert.
Ein kleines Test-Programm `t_strlen.cpp` dafür sieht so aus:

```c++
#include <cassert>

#include "strlen.h"

int main() {
	assert(strlen("") == 0);
	assert(strlen("abc") == 3);
	assert(strlen("a\0b") == 1);
}
```

Damit der Test baut, brauche ich noch den Header `strlen.h`:

```c++
#pragma once

#include <cstddef>

[[nodiscard]] size_t strlen(const char* str);
```

In diesem Progrämmchen benutze ich `assert` für zwei unterschiedliche
Aufgaben:

1. Ich prüfe in der Funktion `strlen`, ob der übergebene Zeiger `nullptr`
   ist und

2. Ich verwende in es `t_strlen.cpp` für einfache Unit-Tests.

Beide Anwendungen habe ich in der freien Wildbahn oft gesehen. Es handelt
sich nicht um ein konstruiertes Problem!

Gerade der erste Fall macht jedoch richtig Probleme: Was passiert, wenn ich
eine Release-Version baue? In diesem Fall wird das `assert` einfach
weggelassen. Falls `strlen` mit einem `nullptr` aufgerufen wird, kann alles
Mögliche passieren. In der Entwicklungsumgebung treten diese Probleme nicht
auf.

Es ist ein bekanntes Problem, dass die Release-Version sich nicht von der
Debug-Version unterscheiden sollte. Es ist jedoch keine Lösung, mit der
Debug-Version in die Produktion zu gehen: oftmals fehlen dort die
Optimierungsschritte, die für einen schnellen Ablauf notwendig sind.

Ich sollte also die Release-Version testen und in der habe ich kein `assert`
mehr. Klar, ich kann es auch dort einbinden, aber das ist dann kein
Standard-C++ mehr. Eine Lösung kann es sein, den Test noch einmal
zusätzlich einzubauen. Ich habe `strlen.cpp` entsprechend angepasst:

```c++
// ...
[[nodiscard]] size_t strlen(const char* str) {
	assert(str);
	if (! str) { return 0; }
	// ...
}
```

Ich verwende [`md-patcher`](https://github.com/itmm/md-patcher), um aus dem
Markdown-Dokument den Source-Code zu extrahieren. Dateinamen werden im
Fließtext als Code-Kommentare angegeben. Die Code-Blöcke probieren, den
angegebenen Text in das bestehende Programm zu integrieren.

Eine besondere Rolle erhalten dabei die Kommentare `// ...`. Sie kopieren
alle Zeilen aus dem ursprünglichen Programm, bis sie auf die Zeile nach
dem Kommentar stoßen. Das ist etwas unintuitiv, aber einfach zu parsen.
Und mit der Zeit gewöhnt man sich daran.

Die generierten Sourcen sind mit der Markdown-Datei eingecheckt.

## Eine Exception werfen

Das Verhalten ist besser, aber noch nicht gut. Zumindest passiert jetzt nichts
Schlimmes, wenn wir die Funktion mit `nullptr` aufrufen. Aber es wird auch
kein Fehler zurückgeliefert. Ich kann natürlich argumentieren, dass `0` die
perfekt richtige Antwort für den Aufruf mit `nullptr` ist. Aber dann
bräuchte ich den `assert` gar nicht.

Ich will mit dem `assert` ja gerade ausdrücken, dass es ein Fehler ist, die
Funktion mit `nullptr` aufzurufen. Vielleicht ist eine Exception für diesen
Fehlerfall besser geeignet. Ich habe `strlen.cpp` wie folgt angepasst:

```c++
#include <cassert>
#include <stdexcept>
// ...
[[nodiscard]] size_t strlen(const char* str) {
	assert(str);
	if (! str) { throw std::invalid_argument { "must not be nullptr" }; }
	#if 0 // don't return default value
	if (! str) { return 0; }
	#endif // don't return default value
	// ...
}
```

Das `#if 0` ist ein Workaround für mein Program
[`md-patcher`](https://github.com/itmm/md-patcher).  Momentan gibt
es keine Möglichkeit, bestehende Zeilen zu löschen. Wenn Sie jedoch mit
einem `#if 0` eingeklammert werden, werden Sie nicht mehr generiert. Der
resultierende Code ist sauber. Im Markdown sieht es jedoch nicht so toll
aus. Und man muss bei der weiteren Verwendung der Datei im Hinterkopf behalten,
dass es diese Kommentare immer noch gibt! Sie müssen ggf. übersprungen werden.

Im nächsten Schritt, packe ich das Werfen der Exception in ein Makro, das dann
`assert` ersetzen kann. Hier ist dazu der Header `solid/require.h`:

```c++
#pragma once

#include <stdexcept>

namespace solid::require {
	class Error: public std::logic_error {
		public:
			Error(const std::string& what): 
				std::logic_error { what } { }
	};
}

#define require(...) do { if (!(__VA_ARGS__)) { \
	require_failed(__FILE__, __LINE__, #__VA_ARGS__) } } while (false)

#define require_failed(FILE, LINE, EXPR) require_failed_2(FILE, LINE, EXPR)

#define require_failed_2(FILE, LINE, EXPR) \
	std::string what { FILE ":" }; \
	what += #LINE; \
	what += " assertion failed: "; \
	what += EXPR; \
	throw solid::require::Error { what };
```

Meinen stabilen Code-Fragmenten habe ich den Namensraum `solid` spendiert. Da
es neben der Exception bald noch eine weitere Klasse gibt, habe ich diesem
Artikel einen eigenen Sub-Namensraum `solid::require` spendiert.

Das Zusammensetzen der Nachricht vollführe ich in mehreren Schritten. Die Idee
dahinter ist, dass es vermutlich mehrere `require`-Aufrufe in einer Datei
gibt. Daher habe ich die Zeichenketten, die vermutlich mehrfach vorkommen,
nicht zusammengesetzt. Dadurch spare ich Konstanten-Platz, wenn es auch etwas
mehr Code bedeutet.

Mit dem Makro kann ich dann die Funktion in `strlen.cpp` kompakter beschreiben:

```c++
#if 0 // don't use assert
#include <cassert>
#include <stdexcept>

#endif // don't use assert
#include "solid/require.h"
#include "strlen.h"

[[nodiscard]] size_t strlen(const char* str) {
	#if 0 // don't use assert and exception
	assert(str);
	if (! str) { throw std::invalid_argument { "must not be nullptr" }; }
	// ...
	#endif // don't return default value
	#endif // don't use assert and exception
	require(str);
	// ...
}
```

Eine ähnliche Anpassung habe ich in `t_strlen.cpp` durchgeführt:

```c++
#if 0
#include <cassert>

#endif
#include "solid/require.h"
#include "strlen.h"

int main() {
	#if 0 // use require instead of assert
	assert(strlen("") == 0);
	assert(strlen("abc") == 3);
	assert(strlen("a\0b") == 1);
	#endif // use require instead of assert
	require(strlen("") == 0);
	require(strlen("abc") == 3);
	require(strlen("a\0b") == 1);
}
```

Zusätzlich kann ich nun Fehlschläge explizit testen. Ein Aufruf von `assert`
ist mir immer um die Ohren geflogen. Aber die Exception kann ich abfangen:

```c++
// ...
#include "strlen.h"

void test_null_strlen() {
	bool got_exception { false };
	try {
		std::ignore = strlen(nullptr);
	} catch (const solid::require::Error&) { got_exception = true; }
	require(got_exception);
}
// ...
	require(strlen("a\0b") == 1);
	test_null_strlen();
// ...
```

Mit dem registrieren eines globalen Handlers kann ich die Ausgabe noch ein
wenig verbessern. Dazu greife ich in `solid/require.h` auf eine globale
Variable zu. Damit stelle ich sicher, dass der Konstruktor der globalen
Variable rechtzeitig aufgerufen wird:

```c++
// ...
namespace solid::require {
	class Global_Handler {
		public:
			Global_Handler();
			const std::string& operator()(
				const std::string& what
			) { return what; }
	};

	class Error: public std::logic_error {
		public:
			Error(const std::string& what): 
				#if 0 // don't initialize without handler
				std::logic_error { what } { }
				#endif // don't initialize without handler
				std::logic_error { handler_(what) } { }
		private:
			static Global_Handler handler_;
	};
}
// ...
```

In der Datei `solid/require.cpp` registriere ich im Konstruktor des Handlers
einen Exception-Handler:

```c++
#include <iostream>

#include "require.h"

using namespace solid::require;

solid::require::Global_Handler::Global_Handler() {
	std::set_terminate([]() -> void {
		try {
			std::rethrow_exception(std::current_exception());
		}
		catch (const Error& err) {
			std::cerr << err.what() << "\n";
			std::exit(EXIT_FAILURE);
		}
	});
}

Global_Handler solid::require::Error::handler_ { };
```

Damit habe ich ganz ohne irgendwelche Änderungen am Haupt-Programm eine
minimale Ausgabe produziert. Und das sowohl in der Debug- als auch der
Release-Version.

Die Ausgabe habe ich so gewählt, dass Editoren die Zeile als Fehlermeldung
parsen können und direkt in die richtige Zeile springen können.

Wäre schon ganz gut, wenn es nicht noch eine weitere Verbesserung gäbe …


## Bessere Typen

Ich sehe die Schwäche der Implementierung von `strlen` im Parameter-Typ.
Wenn ich bereits dem Compiler sage, dass der Parameter nicht `nullptr` sein
darf, kann ich den Test zur Laufzeit an dieser Stelle vermeiden. Die
Verwendung der neuen Klasse in `strlen.cpp` stelle ich mir so vor:

```c++
#if 0 // don't use require
// ...
#include "solid/require.h"
#endif // don't use require
#include "string-literal.h"
// ...

#if 0 // don't use raw c string
[[nodiscard]] size_t strlen(const char* str) {
#endif // don't use raw c string
[[nodiscard]] size_t strlen(const String_Literal& str) {
	#if 0 // don't use require
	// ...
	require(str);
	#endif // don't use require
	// ...
	for (; *cur; ++cur) { }
	#if 0 // don't use raw pointers
	return cur - str;
	#endif // don't use raw pointers
	return cur.ptr() - str.ptr();
}
```

Ich habe den neuen Typ in `string-literal.h` so definiert:

```c++
#pragma once

#include "solid/require.h"

class String_Literal {
	public:
		explicit String_Literal(const char* str = ""): str_ { str } {
			require(str);
		}

		char operator*() const { return *str_; }
		String_Literal& operator++() {
			require(*str_); ++str_; return *this;
		}
		const char* ptr() const { return str_; }
	private:
		const char* str_;
};
```

Der Header `strlen.h` muss ebenfalls an die neue Signatur angepasst werden:

```c++
// ...
#include <cstddef>

#include "string-literal.h"

#if 0 // don't use raw string
[[nodiscard]] size_t strlen(const char* str);
#endif // don't use raw string
[[nodiscard]] size_t strlen(const String_Literal& str);
```

Natürlich müssen auch die Tests in `t_strlen.cpp` angepasst werden:

```c++
// ...
void test_null_strlen() {
	// ...
	try {
	#if 0 // don't use raw pointer
		std::ignore = strlen(nullptr);
	#endif // don't use raw pointer
		std::ignore = strlen(String_Literal { nullptr });
	// ...
}
// ...
int main() {
	#if 0 // don't use raw strings
	// ...
	test_null_strlen();
	#endif // don't use raw strings
	require(strlen(String_Literal { "" }) == 0);
	require(strlen(String_Literal { "abc" }) == 3);
	require(strlen(String_Literal { "a\0b" }) == 1);
	test_null_strlen();
}
```

Meine Herleitung von `solid/require.h` ist dadurch jedoch nicht nutzlos
geworden. Ich verwende immer noch `require`, aber jetzt an einer anderen
Stelle: bei der Erzeugung der `String_Literal`-Instanzen.

Dadurch wurde der Test außerhalb der eigentlichen Funktion gezogen. In der
Funktion benötige ich keinen Code zum Testen der Parameter mehr. Ich kann mich
ganz auf die Umsetzung der eigentlichen Funktion konzentrieren.
