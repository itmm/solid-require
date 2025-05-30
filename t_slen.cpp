#line 214 "README.md"
#include "solid/require.h"
#line 46
#include "slen.h"
#line 232

#include <tuple>

void test_null_slen() {
	bool got_exception { false };
	try {
#line 400
		std::ignore = slen(String_Literal { nullptr });
#line 239
	} catch (const solid::require::Error&) { got_exception = true; }
	require(got_exception);
}
#line 47

int main() {
#line 407
	require(slen(String_Literal { "" }) == 0);
	require(slen(String_Literal { "abc" }) == 3);
	require(slen(String_Literal { "a\0b" }) == 1);
	test_null_slen();
#line 52
}
