#line 137 "README.md"
#pragma once

#include <stdexcept>

#line 237
class Global_Require_Handler {
	public:
		Global_Require_Handler();
		const std::string& operator()(const std::string& what) {
			return what;
		}
};

#line 141
class Require_Error: public std::logic_error {
	public:
		Require_Error(const std::string& what): 
#line 251
			std::logic_error { handler_(what) } { }
	private:
		static Global_Require_Handler handler_;
#line 145
};

#define require(...) do { if (!(__VA_ARGS__)) { \
	require_failed(__FILE__, __LINE__, #__VA_ARGS__) } } while (false)

#define require_failed(FILE, LINE, EXPR) require_failed_2(FILE, LINE, EXPR)

#define require_failed_2(FILE, LINE, EXPR) \
	std::string what { FILE ":" }; \
	what += #LINE; \
	what += " assertion failed: "; \
	what += EXPR; \
	throw Require_Error { what };
