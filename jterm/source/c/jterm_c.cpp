#include <jterm/c/jterm.h>
#include <jterm/cpp/jterm.hpp>

#include <jclib/config.h>


/**
 * @brief Creates a new terminal
*/
extern "C" jterm_Terminal* jterm_new_terminal()
{
	return jterm::new_terminal();
};

/**
 * @brief Deletes a terminal
 * @param _term Terminal instance pointer, will be set to null after deletion
*/
extern "C" void jterm_delete_terminal(jterm_Terminal** _term)
{
	JCLIB_ASSERT(_term);
	jterm::delete_terminal(*_term);
};


extern "C" void jterm_get_color(const jterm_Terminal* _term, jterm_Pos _x, jterm_Pos _y, jterm_Color * _foreground, jterm_Color * _background)
{
	JCLIB_ASSERT(_term);
	jterm::get_color(*_term, _x, _y, _foreground, _background);
};

extern "C" void jterm_set_color(jterm_Terminal * _term, jterm_Pos _x, jterm_Pos _y, jterm_Color _foreground, jterm_Color _background)
{
	JCLIB_ASSERT(_term);
	jterm::set_color(*_term, _x, _y, _foreground, _background);
};



extern "C" void jterm_set(jterm_Terminal * _term, jterm_Char _codepoint, jterm_Pos _x, jterm_Pos _y)
{
	JCLIB_ASSERT(_term);
	jterm::set(*_term, _codepoint, _x, _y);
};
