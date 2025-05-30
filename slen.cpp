#line 327 "README.md"
#include "string-literal.h"
#line 29
#include "slen.h"

#line 334
[[nodiscard]] size_t slen(const String_Literal& str) {
#line 33
	auto cur { str };
	for (; *cur; ++cur) { }
#line 345
	return cur.ptr() - str.ptr();
#line 36
}
