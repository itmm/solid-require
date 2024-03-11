#line 137 "README.md"
#pragma once

#include <exception>
#include <string>

#line 221
class Global_Require_Handler {
	public:
		Global_Require_Handler();
		const std::string& operator()(const std::string& what) {
			return what;
		}
};

#line 142
class Require_Error: public std::exception {
	public:
#line 234
		Require_Error(const std::string& what):
			what_ { handler_(what) } { }
#line 145
		const char* what() const noexcept override {
			return what_.c_str();
		}

	private:
		std::string what_;
#line 240
		static Global_Require_Handler handler_;
#line 151
	
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
