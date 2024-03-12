#line 40 "README.md"

#line 192
#include "require.h"
#line 41
#include "strlen.h"

int main() {
#line 338
	require(strlen(String_Literal { "" }) == 0);
	require(strlen(String_Literal { "abc" }) == 3);
	require(strlen(String_Literal { "a\0b" }) == 1);
#line 47
}
