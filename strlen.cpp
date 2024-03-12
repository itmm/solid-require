#line 26 "README.md"
#include <cstddef>

#line 279
#include "c-str.h"
#line 172

#line 284
[[nodiscard]] size_t strlen(C_Str str) {
#line 30
	size_t count { 0 };
	for (; *str; ++str, ++count) { }
	return count;
}
