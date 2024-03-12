#line 149 "README.md"
#pragma once

#include <stdexcept>

namespace solid::require {
#line 257
	class Global_Handler {
		public:
			Global_Handler();
			const std::string& operator()(
				const std::string& what
			) { return what; }
	};

#line 154
	class Error: public std::logic_error {
		public:
			Error(const std::string& what): 
#line 271
				std::logic_error { handler_(what) } { }
		private:
			static Global_Handler handler_;
#line 158
	};
}

#define require(...) do { if (!(__VA_ARGS__)) { \
	require_failed(__FILE__, __LINE__, #__VA_ARGS__) } } while (false)

#define require_failed(FILE, LINE, EXPR) require_failed_2(FILE, LINE, EXPR)

#define require_failed_2(FILE, LINE, EXPR) \
	std::string what { FILE ":" }; \
	what += #LINE; \
	what += " assertion failed: "; \
	what += EXPR; \
	throw solid::require::Error { what };
