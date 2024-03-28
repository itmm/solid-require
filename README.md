<!-- vim: set spelllang=en noexpandtab: -->
# Problems with `assert()`

At [www.knp.de](http://www.knp.de) I keep my thoughts about programming languages. At the
beginning that will mostly cover C++ and Oberon. That are the languages, that I currently use
most often. It is my goal to give guidance to write more robust and better programs.
Especially C++ is a huge language with a strong focus on execution speed. Other important
topics like safety and stability have queue in afterwards. But on a shaking platform it is
hard to construct a stable building.

It is my goal of these articles to strengthen the base. On this base you can write good
programs with greater ease. If they run a tick more slowly that is a small price to pay for
the greater resulting quality.

If you find an issue or improvement, please open an issue on GitHub. If you can also provide
a fix with a pull request that is most warmly welcome.


## When should I use `assert()`?

The macro `assert` in C++ is a relict from the C days. I use a small program as an example:
calculate the length of a zero-terminated `char` array. Here is my first implementation in
the file `strlen.cpp`:

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

Many thanks to [Zirafinu](https://github.com/Zirafinu) for the optimization of this function.
My first version had an additional counter and no pointer arithmetics. I also wrote a small
test program `t_strlen.cpp`:

```c++
#include <cassert>

#include "strlen.h"

int main() {
	assert(strlen("") == 0);
	assert(strlen("abc") == 3);
	assert(strlen("a\0b") == 1);
}
```

To compile the function and the test program I also wrote the following header file
`strlen.h`:

```c++
#pragma once

#include <cstddef>

[[nodiscard]] size_t strlen(const char* str);
```

In this program I used `assert` for two different tasks:

1. I check in the function `strlen`, if the passed pointer is valid (not `nullptr`) and

2. I use `assert` in `t_strlen.cpp` for simple unit-tests.

Both usages I found multiple times in other programs. It is not a hypothetical problem!

But especially the first usage is problematic: What happens, if I build a release version?
In that case, calls to `assert` will be deleted. The macro will expand to nothing. If I call
`strlen` with a `nullptr` all hell can break loose. But I can't reproduce the problems in
my development environment with my development builds.

It is a known problem that the release version should not differ from the debug version.
But you should not run the debug version in production: elementary optimization steps are
often missing and the execution speed is greatly degraded.

I should use the release version for testing. But then I no longer have the `assert`
function. You *can* enable it nonetheless, but then you left the street of standard C++.
One solution can be an additional test. I can adjust `strlen.cpp` in the following way:

```c++
// ...
[[nodiscard]] size_t strlen(const char* str) {
	assert(str);
	if (! str) { return 0; }
	// ...
}
```

I use [`md-patcher`](https://github.com/itmm/md-patcher), to extract the source code out of
the Markdown document. File names are specified in the main text as inline code blocks.
`md-patcher` tries to combine the code blocks to a compilable program.

A special role have comments of the form `// ...`. They act as a wildcard. `md-patcher`
skips the lines of the original program until it matches the following line (or until a
line with a shorter indentation occurs). That is kind of strange to look at, but easy to
parse. You will get used to it after a bit of reading.

The generated sources are committed together with the Markdown file in the repository.


## Throw an Exception

The current state of the source code is better, but far from acceptable. At least, nothing
bad happens, when the function is called with `nullptr`. But no error state is returned.
I can argue, that `0` is the perfectly correct answer for calling `strlen` with `nullptr`.
But `assert` means something different.

With `assert` I state explicitly, that it is an error to call `strlen` with `nullptr`.
Maybe an exception is the more appropriate way to handle this error condition in
`strlen.cpp`:

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

The `#if 0` is a workaround of [`md-patcher`](https://github.com/itmm/md-patcher). Currently
there is no way to delete lines directly. But if I comment them out this way, they will be
no longer generated. It looks not great in Markdown and you have to keep in mind, that the
lines are still there (and must be skipped with `// ...` eventually.

As a next step, I put the throwing of the exception into a macro that will replace `assert`.
A macro is necessary to collect the current source code file name and line number. I put it
into the header `include/solid/require.h`:

```c++
#pragma once

#include <stdexcept>
#include <string>

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

I put my code fragments that help to improve software stability in the namespace `solid`. As
there will be more classes beside the exception soon, I put the exception into a special
namespace `solid::require`.

I perform the combination of the message in multiple steps. That is, because there are
hopefully multiple `require` calls in the same source file. Therefore I put all characters
that are unique per source file into one string and concatenate the line number afterwards.
That way, I do not need so much space for string constants. But the result is a bit more
code.

Now I can use the macro in `strlen.cpp`:

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

A similar adaption I perform in `t_strlen.cpp`:

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

I can now also test, that `strlen` fails as expected by catching the exception:

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

For a nicer output I register a global exception handler. To get the right moment to register
the handler, I access in `include/solid/require.h` a global variable on every test. That
assures that the constructor of the global variable is called early enough and can register
the handler:

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

In `solid/require.cpp` I register the handler in the constructor of the global variable:

```c++
#include <iostream>

#include "solid/require.h"

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

Without any modification in them main program I have reduced the output to a readable
amount. And that works both in the debug and release version.

I choose the output, so that editors and IDEs can interpret it as an error message and jump
directly to the line of the `require` call.

I am pretty happy with this code. But there is one more thing …


## Better Types

I see in the type of the parameter of `strlen` a problem. It would be better, if I can say,
that this type is not allowed to be `nullptr`. Then the compiler can check and not allow to
call this function in an insecure way. I use the new class `String_Literal` in `strlen.cpp`
to achieve this. I no longer need the `require` check any more:

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

I defined the class `String_Literal` in `string-literal.h` in the following way:

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

I have changed the signature of `strlen`. So, I have to adjust it in `strlen.h` also:

```c++
// ...
#include <cstddef>

#include "string-literal.h"

#if 0 // don't use raw string
[[nodiscard]] size_t strlen(const char* str);
#endif // don't use raw string
[[nodiscard]] size_t strlen(const String_Literal& str);
```

And I have to adjust the test-cases in `t_strlen.cpp`:

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

But I still need the `require` function. But now in the construction of the `String_Literal`.
That makes the code of the `strlen` function clearer: I have moved the test out of the
function and declared with the new parameter type the constraints I impose on it.

I think, that is a nice version to close this article.
