#line 41 "README.md"

#line 203
#include "solid/require.h"
#line 42
#include "strlen.h"

#line 225
void test_null_strlen() {
	bool got_exception { false };
	try {
#line 382
		std::ignore = strlen(String_Literal { nullptr });
#line 229
	} catch (const solid::require::Error&) {
		got_exception = true;
	}
	require(got_exception);
}
#line 44
int main() {
#line 392
	require(strlen(String_Literal { "" }) == 0);
	require(strlen(String_Literal { "abc" }) == 3);
	require(strlen(String_Literal { "a\0b" }) == 1);
	test_null_strlen();
#line 48
}
