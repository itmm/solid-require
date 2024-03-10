#line 38 "README.md"
#include <cassert>

#include "strlen.h"

int main() {
	assert(strlen("") == 0);
	assert(strlen("abc") == 3);
	assert(strlen("a\0b") == 1);
}
