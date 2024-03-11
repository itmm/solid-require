#line 26 "README.md"
#include <cstddef>

#line 171
#include "require.h"

#line 28
[[nodiscard]] size_t strlen(const char* str) {
#line 179
	require(str);
#line 30
	size_t count { 0 };
	while (*str++) { ++count; }
	return count;
}
