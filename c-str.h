#line 296 "README.md"
#pragma once

#include "require.h"

class C_Str {
	public:
		explicit C_Str(const char* str = ""): str_ { str } {
			require(str);
		}

		char operator*() const { return *str_; }
		C_Str& operator++() {
			require(*str_); ++str_; return *this;
		}
	private:
		const char* str_;
};
