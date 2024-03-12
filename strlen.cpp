#line 329 "README.md"
#include "string-literal.h"
#line 29
#include "strlen.h"

#line 335
[[nodiscard]] size_t strlen(const String_Literal& str) {
#line 33
	auto cur { str };
	for (; *cur; ++cur) { }
#line 345
	return cur.ptr() - str.ptr();
#line 36
}
