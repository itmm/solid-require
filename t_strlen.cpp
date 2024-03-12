#line 40 "README.md"

#line 192
#include "require.h"
#line 41
#include "strlen.h"

int main() {
#line 338
	require(strlen(C_Str { "" }) == 0);
	require(strlen(C_Str { "abc" }) == 3);
	require(strlen(C_Str { "a\0b" }) == 1);
#line 47
}
