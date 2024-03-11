#line 40 "README.md"

#line 198
#include "require.h"
#line 41
#include "strlen.h"

int main() {
#line 207
	require(strlen("") == 0);
	require(strlen("abc") == 3);
	require(strlen("a\0b") == 2);
#line 47
}
