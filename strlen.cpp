#line 319 "README.md"
#include "string-literal.h"
#line 28
#include "strlen.h"

#line 325
[[nodiscard]] size_t strlen(const String_Literal& str) {
#line 32
	auto cur { str };
	for (; *cur; ++cur) { }
#line 335
	return cur.ptr() - str.ptr();
#line 35
}
