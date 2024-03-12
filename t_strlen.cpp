#line 43 "README.md"

#line 205
#include "solid/require.h"
#line 44
#include "strlen.h"

#line 227
void test_null_strlen() {
	bool got_exception { false };
	try {
#line 384
		std::ignore = strlen(String_Literal { nullptr });
#line 231
	} catch (const solid::require::Error&) {
		got_exception = true;
	}
	require(got_exception);
}
#line 46
int main() {
#line 394
	require(strlen(String_Literal { "" }) == 0);
	require(strlen(String_Literal { "abc" }) == 3);
	require(strlen(String_Literal { "a\0b" }) == 1);
	test_null_strlen();
#line 50
}
