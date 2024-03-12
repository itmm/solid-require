#line 26 "README.md"
#include <cstddef>

#line 318
#include "string-literal.h"
#line 185

#line 323
[[nodiscard]] size_t strlen(const String_Literal& str) {
#line 30
	auto cur { str };
	for (; *cur; ++cur) { }
#line 333
	return cur.ptr() - str.ptr();
#line 33
}
