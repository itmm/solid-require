#line 40 "README.md"

#line 192
#include "require.h"
#line 41
#include "strlen.h"

#line 214
void test_null_strlen() {
	bool got_exception { false };
	try {
#line 367
		std::ignore = strlen(String_Literal { nullptr });
#line 218
	} catch (const Require_Error&) {
		got_exception = true;
	}
	require(got_exception);
}
#line 43
int main() {
#line 377
	require(strlen(String_Literal { "" }) == 0);
	require(strlen(String_Literal { "abc" }) == 3);
	require(strlen(String_Literal { "a\0b" }) == 1);
	test_null_strlen();
#line 47
}
