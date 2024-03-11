#line 26 "README.md"
#include <cstddef>

#line 178
#include "require.h"

#line 28
[[nodiscard]] size_t strlen(const char* str) {
#line 186
	require(str);
#line 30
	size_t count { 0 };
	while (*str++) { ++count; }
	return count;
}
