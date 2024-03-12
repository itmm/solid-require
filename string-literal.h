#line 296 "README.md"
#pragma once

#include "require.h"

class String_Literal {
	public:
		explicit String_Literal(const char* str = ""): str_ { str } {
			require(str);
		}

		char operator*() const { return *str_; }
		String_Literal& operator++() {
			require(*str_); ++str_; return *this;
		}
	private:
		const char* str_;
};
