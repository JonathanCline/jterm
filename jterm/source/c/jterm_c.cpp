#include <jterm/c/jterm.h>

#include "terminal/terminal.hpp"

#include <jclib/config.h>



/**
 * @brief Terminal instance data
*/
extern "C"
{
	struct jterm_Terminal : public jterm::terminal_instance
	{
		using jterm::terminal_instance::terminal_instance;
	};
};

/**
 * @brief Creates a new terminal
*/
extern "C" struct jterm_Terminal* jterm_new_terminal()
{
	return static_cast<struct jterm_Terminal*>(jterm::new_terminal());
};

/**
 * @brief Deletes a terminal
 * @param _term Terminal instance pointer, will be set to null after deletion
*/
extern "C" void jterm_delete_terminal(struct jterm_Terminal** _term)
{
	JCLIB_ASSERT(_term);
	delete *_term;
	*_term = nullptr;
};
