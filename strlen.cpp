#line 24 "README.md"
#include <cassert>
#include <cstddef>

[[nodiscard]] size_t strlen(const char* str) {
	assert(str);
#line 90
	if (! str) { return 0; }
#line 29
	size_t count { 0 };
	while (*str++) { ++count; }
	return count;
}
