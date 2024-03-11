#line 239 "README.md"
#include <iostream>

#include "require.h"

Global_Require_Handler::Global_Require_Handler() {
	std::set_terminate([]() -> void {
		try {
			std::rethrow_exception(std::current_exception());
		}
		catch (const Require_Error& err) {
			std::cerr << err.what() << "\n";
			std::exit(EXIT_FAILURE);
		}
	});
}

Global_Require_Handler Require_Error::handler_ { };
