#line 326 "README.md"
#include "string-literal.h"
#line 27
#include "strlen.h"

#line 332
[[nodiscard]] size_t strlen(const String_Literal& str) {
#line 31
	auto cur { str };
	for (; *cur; ++cur) { }
#line 342
	return cur.ptr() - str.ptr();
#line 34
}
