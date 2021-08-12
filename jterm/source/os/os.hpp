#pragma once

#if defined(_WIN32)
#include "win/terminal.hpp"
#else
#error "Bug me to add support, or if you are me, stop being lazy"
#endif

namespace jterm
{
	namespace os {};

	// If this causes an error, there is no terminal type for the detected target os
	using os_terminal = os::terminal;
};

