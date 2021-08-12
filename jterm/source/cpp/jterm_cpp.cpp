#include <jterm/cpp/jterm.hpp>

#include "terminal/terminal.hpp"

#include <jclib/config.h>

#include <new>

namespace jterm
{

	terminal_instance* new_terminal()
	{
		static bool first_call = true;

		if (first_call) JCLIB_UNLIKELY
		{
			first_call = false;
			return new terminal_instance{ terminal_instance::allow_reuse };
		}
		else
		{
			return new terminal_instance{};
		};
	};

	void delete_terminal(terminal_instance*& _term)
	{
		delete _term;
		_term = nullptr;
	};

	void write(terminal_instance& _term, std::string_view _str, int _x, int _y)
	{
		auto& _osTerm = _term.term_;

		_osTerm.set_cursor_pos(static_cast<size_t>(_x), static_cast<size_t>(_y));
		_osTerm.write(_str);
	};


	void refresh(terminal_instance& _term)
	{
		auto& _osTerm = _term.term_;
		_osTerm.swap_buffers();
		_osTerm.get_input_events();
	};



	void set_foreground_color(terminal_instance& _term, uint8_t r, uint8_t g, uint8_t b)
	{
		auto& _osTerm = _term.term_;
		_osTerm.set_foreground_color({ r, g, b });
	};
	void set_background_color(terminal_instance& _term, uint8_t r, uint8_t g, uint8_t b)
	{
		auto& _osTerm = _term.term_;
		_osTerm.set_background_color({ r, g, b });
	};

	void put(terminal_instance& _term, char _char, int _x, int _y)
	{
		auto& _osTerm = _term.term_;
		_osTerm.put(_char, static_cast<size_t>(_x), static_cast<size_t>(_y));
	};


	// @param _userParam Passed to the callback function on invocation
	void set_callback(terminal_instance& _term, text_callback_fn _callback, void* _userParam)
	{
		auto& _osTerm = _term.term_;
		_osTerm.callbacks().on_text = callback{ _callback, _userParam };
	};


};
