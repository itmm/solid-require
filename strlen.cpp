#line 26 "README.md"
#include <cstddef>

#line 301
#include "string-literal.h"
#line 172

#line 306
[[nodiscard]] size_t strlen(const String_Literal& str) {
#line 30
	auto cur { str };
	for (; *cur; ++cur) { }
#line 316
	return cur.ptr() - str.ptr();
#line 33
}
