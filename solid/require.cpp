#line 277 "README.md"
#include <iostream>

#include "solid/require.h"

using namespace solid::require;

solid::require::Global_Handler::Global_Handler() {
	std::set_terminate([]() -> void {
		try {
			std::rethrow_exception(std::current_exception());
		}
		catch (const Error& err) {
			std::cerr << err.what() << "\n";
			std::exit(EXIT_FAILURE);
		}
	});
}

Global_Handler solid::require::Error::handler_ { };
