<!-- vim: set spelllang=de noexpandtab: -->
# Das Problem mit `assert()`

In diesem Blog halte ich meine Gedanken zu Programmiersprachen fest.
Hauptsächlich werden das erstmal C++ und Oberon sein. Mit diesen Sprachen
befasse ich mich im Augenblick am intensivsten. Das Ziel ist es, kleine
Handreichungen zu geben, um stabilere und bessere Programme zu entwickeln.
Die Sprachen sind riesig und gerade C++ hat einen sehr starken Fokus auf
Geschwindigkeit. Dafür werden oft Sicherheit und Stabilität geopfert. Auf
einer schwankenden Plattform kann aber nur sehr schwierig etwas Stabiles
errichtet werden.

Daher ist der Sinn dieser Beiträge, das Fundament zu festigen. Damit können 
wir leichter gute Programme schreiben. Wenn sie dann etwas langsamer Laufen,
ist das ein geringer Preis für die resultierende Qualitätssteigerung.


## Wann sollte ich `assert()` verwenden

Das Makro `assert` ist in C++ ein Überbleibsel aus den C Tagen. Ich verwende
ein kleines Programm als durchgehendes Beispiel: die Ermittlung der Länge
einer Zeichenfolge. Hier zuerst die Implementierung in der Datei `strlen.cpp`:

```c++
#include <cassert>
#include <cstddef>

[[nodiscard]] size_t strlen(const char* str) {
	assert(str);
	size_t count { 0 };
	while (*str++) { ++count; }
	return count;
}
```

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

[[nodiscard]] size_t strlen(const char* a);
```

In diesem Progrämmchen benutze ich `assert` für zwei unterschiedliche
Aufgaben:

1. Ich prüfe in der Funktion `strlen`, ob der übergebene Zeiger `nullptr`
   ist und

2. Ich verwende in `t_strlen.cpp` für einfache Unit-Tests.

Beide Anwendungen habe ich in der freien Wildbahn oft gesehen. Es handelt
sich also nicht um ein konstruiertes Problem.

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
// ...
#include <cstddef>
#include <stdexcept>
// ...
[[nodiscard]] size_t strlen(const char* str) {
	assert(str);
	if (! str) { throw std::invalid_argument { "must not be nullptr" }; }
	#if false
	if (! str) { return 0; }
	#endif
	// ...
}
```
