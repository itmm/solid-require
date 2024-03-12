#line 214 "README.md"
#include "solid/require.h"
#line 44
#include "strlen.h"

#line 236
void test_null_strlen() {
	bool got_exception { false };
	try {
#line 393
		std::ignore = strlen(String_Literal { nullptr });
#line 240
	} catch (const solid::require::Error&) { got_exception = true; }
	require(got_exception);
}
#line 46
int main() {
#line 402
	require(strlen(String_Literal { "" }) == 0);
	require(strlen(String_Literal { "abc" }) == 3);
	require(strlen(String_Literal { "a\0b" }) == 1);
	test_null_strlen();
#line 50
}
