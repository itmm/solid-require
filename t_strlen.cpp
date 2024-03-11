#line 40 "README.md"

#line 191
#include "require.h"
#line 41
#include "strlen.h"

int main() {
#line 200
	require(strlen("") == 0);
	require(strlen("abc") == 3);
	require(strlen("a\0b") == 1);
#line 47
}
