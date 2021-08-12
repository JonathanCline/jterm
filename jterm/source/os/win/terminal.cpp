#include "os/win/terminal.hpp"

#include <jclib/config.h>
#include <jclib/ranges.h>



namespace jterm::os
{

	void notify_error(const char* _message, const char* _caption)
	{
		MessageBoxA(NULL, _message, _caption, MB_OK);
	};

};


namespace jterm::os
{


	namespace
	{
		inline auto has_input(HANDLE _inBuf)
		{
			constexpr size_t record_len = 1;
			
			DWORD _count = 0;
			INPUT_RECORD _record[record_len]{};
			if (!PeekConsoleInputA(_inBuf, _record, record_len, &_count))
			{
				notify_debug("Failed to peek console input", here());
				JCLIB_ABORT();
			};

			return _count != 0;
		};
	};




	terminal_io_state terminal::get_std_handles()
	{
		const auto _in  = GetStdHandle(STD_INPUT_HANDLE);
		const auto _out = GetStdHandle(STD_OUTPUT_HANDLE);
		const auto _err = GetStdHandle(STD_ERROR_HANDLE);
		return terminal_io_state{ _in, _out, _err };
	};

	void terminal::init_io_state()
	{
		this->io_ = this->get_std_handles();
	};

	







	terminal_options_state terminal::get_options() const
	{
		const auto& _outbuf = this->io_.out_;

		CONSOLE_SCREEN_BUFFER_INFO _info{};
		DWORD _mode{};

		if (!GetConsoleScreenBufferInfo(_outbuf, &_info))
		{
			JCLIB_ABORT();
		};
		if (!GetConsoleMode(_outbuf, &_mode))
		{
			JCLIB_ABORT();
		};

		terminal_options_state _state{};
		_state.colors = _info.wAttributes;
		_state.mode = _mode;
		return _state;
	};

	void terminal::set_options(terminal_options_state _opState)
	{
		const auto& _outbuf = this->io_.out_;

		if (!SetConsoleTextAttribute(_outbuf, _opState.colors))
		{
			JCLIB_ABORT();
		};
		if (!SetConsoleMode(_outbuf, _opState.mode))
		{
			JCLIB_ABORT();
		};

	};

	void terminal::set_cursor_pos(size_t _x, size_t _y)
	{
		auto& _cursor = this->cursor_;
		auto& _buffer = this->buffer_;

		_cursor = _buffer.to_index(_x, _y);
	};


	void terminal::write(std::string_view _str)
	{
		auto& _cursor = cursor_;
		auto& _buffer = this->buffer_.data_;

		const auto& _fg = this->foreground_;
		const auto& _bk = this->background_;

		auto it = jc::next(jc::begin(_buffer), _cursor);

		for (auto& v : _str)
		{
			it->character = v;
			it->fgcolor = _fg;
			it->bkcolor = _bk;
			++it;
			++_cursor;
		};

	};



	void terminal::put(char _char, size_t _x, size_t _y)
	{
		auto& _buffer = this->buffer_;
		const auto _index = _buffer.to_index(_x, _y);
		
		auto& _cell = _buffer.at(_index);

		_cell.character = _char;
		_cell.fgcolor = this->foreground_;
		_cell.bkcolor = this->background_;
	};


	void terminal::get_input_events()
	{
		auto& _inBuf = this->io_.in_;
		while (has_input(_inBuf))
		{
			constexpr size_t record_len_v = 16;
			
			INPUT_RECORD _record[record_len_v]{};
			DWORD _count = 0;

			if (!ReadConsoleInputA(_inBuf, _record, record_len_v, &_count))
			{
				const auto _err = GetLastError();
				notify_debug("Failed to read terminal input events", here());
				JCLIB_ABORT();
			};
			
			auto& _callbacks = this->callbacks();
			for (auto& v : _record)
			{
				if (v.EventType == KEY_EVENT)
				{
					const auto& _ev = v.Event.KeyEvent;
				
					if (_callbacks.on_text && _ev.bKeyDown && std::isprint(_ev.uChar.AsciiChar))
					{
						_callbacks.on_text(_ev.uChar.AsciiChar);
					};
				};
			};

		};




	};


	void terminal::swap_buffers()
	{
		auto& _outbuf = this->io_.out_;
		auto& _buffer = this->buffer_;

		auto& _chars = this->characters_;
		auto& _cols = this->colors_;

		{
			const auto _bufferLen = _buffer.size();
			if (_chars.size() != _bufferLen)
			{
				_chars.resize(_bufferLen);
				_cols.resize(_bufferLen);
			};
		};

		{
			auto& _bufferData = _buffer.data_;

			auto it = _bufferData.begin();
			const auto _end = _bufferData.end();

			auto _charIter = _chars.begin();
			auto _colorIter = _cols.begin();
			_colorIter->len = 0;

			for (it; it != _end; ++it)
			{
				*_charIter = it->character;
				_charIter++;

				if (it->fgcolor == _colorIter->fgcolor && it->bkcolor == _colorIter->bkcolor)
				{
					++_colorIter->len;
				}
				else
				{
					++_colorIter;
					_colorIter->fgcolor = it->fgcolor;
					_colorIter->bkcolor = it->bkcolor;
					_colorIter->len = 1;
				};
			};
		};



		constexpr COORD cursor_start_pos_v{ 0, 0 };
		if (!SetConsoleCursorPosition(_outbuf, cursor_start_pos_v))
		{
			notify_debug("failed to reset terminal cursor position", here());
			JCLIB_ABORT();
		};

		size_t _cursor = 0;
		for (auto& _span : _cols)
		{
			{
				const auto [x, y] = _buffer.to_position(_cursor);
				this->set_cursor_pos(x, y);
			};

			const auto _count = _span.len;
			const auto& _fgcol = _span.fgcolor;
			const auto& _bkcol = _span.bkcolor;

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

			if (!SetConsoleTextAttribute(_outbuf, _attrib))
			{
				notify_debug("failed to set terminal text color", here());
				JCLIB_ABORT();
			};

			if (!WriteFile(_outbuf, _chars.data() + _cursor, _count, NULL, NULL))
			{
				notify_debug("failed to write buffer to the terminal", here());
				JCLIB_ABORT();
			};

			_cursor += _count;
		};

	};



	void terminal::set_foreground_color(terminal_cell::color_type _col)
	{
		this->foreground_ = _col;
	};
	void terminal::set_background_color(terminal_cell::color_type _col)
	{
		this->background_ = _col;
	};





	namespace
	{
		// Indirection to open a new terminal window
		inline bool new_terminal() noexcept
		{
			return AllocConsole();
		};

		inline auto get_console_mode(HANDLE _buf) noexcept
		{
			DWORD _mode{};
			if (!GetConsoleMode(_buf, &_mode))
			{
				notify_debug("failed to get console mode", here());
				JCLIB_ABORT();
			};
			return _mode;
		};
		inline auto set_console_mode(HANDLE _buf, DWORD _mode) noexcept
		{
			const auto _result = SetConsoleMode(_buf, _mode);
			if (!_result)
			{
				const auto _err = GetLastError();

				notify_debug("failed to set console mode", here());
				JCLIB_ABORT();
			};
		};


		inline auto get_cursor_info(HANDLE _out) noexcept
		{
			CONSOLE_CURSOR_INFO _info{};
			if (!GetConsoleCursorInfo(_out, &_info))
			{
				notify_debug("failed to get cursor info", here());
				JCLIB_ABORT();
			};
			return _info;
		};

		inline void set_cursor_visiblity(HANDLE _buf, bool _visible) noexcept
		{
			CONSOLE_CURSOR_INFO _info = get_cursor_info(_buf);
			_info.bVisible = _visible;
			if (!SetConsoleCursorInfo(_buf, &_info))
			{
				notify_debug("failed to set cursor info", here());
				JCLIB_ABORT();
			};
		};

		inline void hide_cursor(HANDLE _buf) noexcept
		{
			set_cursor_visiblity(_buf, false);
		};

		inline void show_cursor(HANDLE _buf) noexcept
		{
			set_cursor_visiblity(_buf, true);
		};

		inline void set_immediate_input(HANDLE _buf) noexcept
		{
			auto _mode = get_console_mode(_buf);
			_mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
			set_console_mode(_buf, _mode);
		};


	};

	terminal::terminal(allow_reuse_t)
	{
		new_terminal();

		this->init_io_state();
		hide_cursor(this->io_.out_);

		CONSOLE_SCREEN_BUFFER_INFO _info{};
		GetConsoleScreenBufferInfo(this->io_.out_, &_info);
		this->buffer_.resize(_info.dwSize.X, this->buffer_.height());

		set_immediate_input(this->io_.in_);

	};
	terminal::terminal()
	{
		if (!new_terminal())
		{
			notify_debug("cannot open multiple terminals", here());
			JCLIB_ABORT();
		};

		this->init_io_state();
		hide_cursor(this->io_.out_);

		CONSOLE_SCREEN_BUFFER_INFO _info{};
		GetConsoleScreenBufferInfo(this->io_.out_, &_info);
		this->buffer_.resize(_info.dwSize.X, this->buffer_.height());

		set_immediate_input(this->io_.in_);
	};


	terminal::~terminal()
	{
		if (!FreeConsole())
		{
			notify_debug("failed to close terminal", here());
			JCLIB_ABORT();
		};
	};

};

