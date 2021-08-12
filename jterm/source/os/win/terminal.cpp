#include "terminal.hpp"

#include <jclib/config.h>
#include <memory>

#include <Windows.h>



struct jterm_Terminal
{
	HANDLE out;
	HANDLE in;
	HANDLE err;


};


namespace jterm
{
	namespace
	{
		inline auto make_color_attribute(jterm_Color _fgcol, jterm_Color _bkcol)
		{
			WORD _attrib{};

			if (_fgcol.r != 0)
			{
				_attrib |= FOREGROUND_RED;
			};
			if (_fgcol.g != 0)
			{
				_attrib |= FOREGROUND_GREEN;
			};
			if (_fgcol.b != 0)
			{
				_attrib |= FOREGROUND_BLUE;
			};

			if (_bkcol.r != 0)
			{
				_attrib |= BACKGROUND_RED;
			};
			if (_bkcol.g != 0)
			{
				_attrib |= BACKGROUND_GREEN;
			};
			if (_bkcol.b != 0)
			{
				_attrib |= BACKGROUND_BLUE;
			};

			return _attrib;
		};
	};



	/**
	 * @brief Creates a new terminal
	*/
	jterm_Terminal* new_terminal()
	{
		std::unique_ptr<jterm_Terminal> _out{ new jterm_Terminal{} };

		auto& _term = *_out;
		_term.out = GetStdHandle(STD_OUTPUT_HANDLE);
		_term.in  = GetStdHandle(STD_INPUT_HANDLE);
		_term.err = GetStdHandle(STD_ERROR_HANDLE);

		return _out.release();
	};

	/**
	 * @brief Deletes a terminal
	 * @param _term Terminal instance pointer, will be set to null after deletion
	*/
	void delete_terminal(jterm_Terminal*& _term)
	{
		delete _term;
		_term = nullptr;
	};


	void get_color(const jterm_Terminal& _term, jterm_Pos x, jterm_Pos y, jterm_Color* _foreground, jterm_Color* _background)
	{
		COORD _coord{ x, y };
		auto& _outbuf = _term.out;

		DWORD _count = 0;
		WORD _attrib = 0;
	
		if (!ReadConsoleOutputAttribute(_outbuf, &_attrib, 1, _coord, &_count)) JCLIB_UNLIKELY
		{
			JCLIB_ABORT();
		};

		if (_foreground)
		{
			auto& [r, g, b, a] = *_foreground;
			
			if ((_attrib & FOREGROUND_RED) != 0)
			{
				r = 255;
			};
			if ((_attrib & FOREGROUND_BLUE) != 0)
			{
				b = 255;
			};
			if ((_attrib & FOREGROUND_GREEN) != 0)
			{
				g = 255;
			};
		};
		if (_background)
		{
			auto& [r, g, b, a] = *_background;

			if ((_attrib & BACKGROUND_RED) != 0)
			{
				r = 255;
			};
			if ((_attrib & BACKGROUND_BLUE) != 0)
			{
				b = 255;
			};
			if ((_attrib & BACKGROUND_GREEN) != 0)
			{
				g = 255;
			};
		};

	};
	void set_color(jterm_Terminal& _term, jterm_Pos _x, jterm_Pos _y, jterm_Color _foreground, jterm_Color _background)
	{
		COORD _coord{ _x, _y };
		auto& _outbuf = _term.out;
		const auto _attrib = make_color_attribute(_foreground, _background);

		DWORD _count = 0;
		if (!FillConsoleOutputAttribute(_outbuf, _attrib, 1, _coord, &_count))
		{
			JCLIB_ABORT();
		};
	};







	void fill_color(jterm_Terminal& _term, jterm_Pos _x, jterm_Pos _y, jterm_Pos _w, jterm_Pos _h, jterm_Color _fg, jterm_Color _bg)
	{
		COORD _coord{ _x, _y };
		auto& _outbuf = _term.out;

		DWORD _count = 0;
		WORD _attrib = make_color_attribute(_fg, _bg);

		for (jterm_Pos row = 0; row != _h; ++row)
		{
			if (!FillConsoleOutputAttribute(_outbuf, _attrib, _w, _coord, &_count)) JCLIB_UNLIKELY
			{
				JCLIB_ABORT();
			};
			++_coord.Y;
		};
	};



	void set(jterm_Terminal& _term, jterm_Char _codepoint, jterm_Pos _x, jterm_Pos _y)
	{
		auto& _outbuf = _term.out;
		DWORD _count = 0;

		COORD _cord{ _x, _y };
		if (!SetConsoleCursorPosition(_outbuf, _cord))
		{
			JCLIB_ABORT();
		};

		if (!WriteFile(_outbuf, &_codepoint, 1, &_count, MB_OK))
		{
			JCLIB_ABORT();
		};
	};


};
