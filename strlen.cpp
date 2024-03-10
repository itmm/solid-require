#line 24 "README.md"
#include <cassert>
#include <cstddef>
#line 111
#include <stdexcept>
#line 26

[[nodiscard]] size_t strlen(const char* str) {
	assert(str);
#line 115
	if (! str) { throw std::invalid_argument { "must not be nullptr" }; }
	#if false
#line 90
	if (! str) { return 0; }
#line 118
	#endif
#line 29
	size_t count { 0 };
	while (*str++) { ++count; }
	return count;
}
