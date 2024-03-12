#line 328 "README.md"
#include "string-literal.h"
#line 29
#include "strlen.h"

#line 334
[[nodiscard]] size_t strlen(const String_Literal& str) {
#line 33
	auto cur { str };
	for (; *cur; ++cur) { }
#line 344
	return cur.ptr() - str.ptr();
#line 36
}
