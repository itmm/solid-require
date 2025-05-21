#line 208 "README.md"
#include "solid/require.h"
#line 45
#include "strlen.h"

#line 229
#include <tuple>

void test_null_strlen() {
	bool got_exception { false };
	try {
#line 386
		std::ignore = strlen(String_Literal { nullptr });
#line 235
	} catch (const solid::require::Error&) { got_exception = true; }
	require(got_exception);
}
#line 47
int main() {
#line 395
	require(strlen(String_Literal { "" }) == 0);
	require(strlen(String_Literal { "abc" }) == 3);
	require(strlen(String_Literal { "a\0b" }) == 1);
	test_null_strlen();
#line 51
}
