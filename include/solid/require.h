#line 152 "README.md"
#pragma once

#include <stdexcept>
#include <string>

namespace solid::require {
#line 256
	class Global_Handler {
		public:
			Global_Handler();
			const std::string& operator()(
				const std::string& what
			) { return what; }
	};

#line 158
	class Error: public std::logic_error {
		public:
			Error(const std::string& what): 
#line 270
				std::logic_error { handler_(what) } { }
		private:
			static Global_Handler handler_;
#line 162
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
