#include "jterm/jterm.h"

#if false

#include <memory>

#include <Windows.h>
#undef min
#undef max


#define JTERM_NOTIFY_ERROR(what, err) { MessageBox(NULL, TEXT( what ), TEXT( err ), MB_OK); }



namespace jterm
{
	struct terminal_handle
	{
		HANDLE bout;
		HANDLE bin;
		CONSOLE_SCREEN_BUFFER_INFO buffer_info;
		
		DWORD old_mode;
		WORD  old_colors;
	};

	inline void scroll_screen_buffer(HANDLE h, INT x, SHORT _count)
	{
		SMALL_RECT srctScrollRect, srctClipRect;
		CHAR_INFO chiFill;
		COORD coordDest;
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

		if (!GetConsoleScreenBufferInfo(h, &csbiInfo))
		{
			JTERM_NOTIFY_ERROR("scroll_screen_buffer On call to GetConsoleScreenBufferInfo", "Console Error");
			return;
		};

		srctScrollRect.Left = 0;
		srctScrollRect.Top = _count;
		srctScrollRect.Right = csbiInfo.dwSize.X - (SHORT)x;
		srctScrollRect.Bottom = csbiInfo.dwSize.Y - (SHORT)x;

		coordDest.X = 0;
		coordDest.Y = 0;

		// The clipping rectangle is the same as the scrolling rectangle.
		// The destination row is left unchanged.
		srctClipRect = srctScrollRect;

		// Set the fill character and attributes.
		chiFill.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
		chiFill.Char.AsciiChar = (char)' ';

		// Scroll up
		ScrollConsoleScreenBuffer(
			h,               // screen buffer handle
			&srctScrollRect, // scrolling rectangle
			&srctClipRect,   // clipping rectangle
			coordDest,       // top left destination cell
			&chiFill);       // fill character and color
	};
	
	inline auto scroll_screen_buffer(HANDLE h, INT x)
	{
		return scroll_screen_buffer(h, x, 1);
	};

};

namespace jterm
{
	terminal_handle* new_terminal()
	{
		std::unique_ptr<terminal_handle> _out{ new terminal_handle{} };
		auto& _term = *_out;

		auto& _bout = _term.bout;
		auto& _bin = _term.bin;

		_bin = GetStdHandle(STD_INPUT_HANDLE);
		_bout = GetStdHandle(STD_OUTPUT_HANDLE);
		
		if (_bin == INVALID_HANDLE_VALUE ||
			_bout == INVALID_HANDLE_VALUE)
		{
			JTERM_NOTIFY_ERROR("GetStdHandle", "Console Error");
			return nullptr;
		};

		if (!GetConsoleScreenBufferInfo(_bout, &_term.buffer_info))
		{
			JTERM_NOTIFY_ERROR("GetConsoleScreenBufferInfo", "Console Error");
			return nullptr;
		};

		// Store state
		_term.old_colors = _term.buffer_info.wAttributes;
		if (!GetConsoleMode(_bin, &_term.old_mode))
		{
			JTERM_NOTIFY_ERROR("GetConsoleMode", "Console Error");
			return nullptr;
		};

		// Good creation
		return _out.release();
	};

	void delete_terminal(terminal_handle*& _handle)
	{
		if (_handle)
		{
			auto& _term = *_handle;

			// Restore the original console state

			SetConsoleMode(_term.bin, _term.old_mode);
			SetConsoleTextAttribute(_term.bout, _term.old_colors);
		};

		delete _handle;
		_handle = nullptr;
	};

};


namespace jterm
{

	cursor_pos terminal::get_cursor_pos() const
	{
		cursor_pos _out{};

		CONSOLE_SCREEN_BUFFER_INFO _info;
		if (!GetConsoleScreenBufferInfo(this->term_->bout, &_info))
		{
			std::terminate();
		};

		const auto& _cursor = _info.dwCursorPosition;

		_out.x = _cursor.X;
		_out.y = _cursor.Y;

		return _out;
	};
	void terminal::set_cursor_pos(cursor_pos _pos)
	{
		COORD _cursor{};
		_cursor.X = _pos.x;
		_cursor.Y = _pos.y;

		if (!SetConsoleCursorPosition(this->term_->bout, _cursor))
		{
			std::terminate();
		};
	};

	void terminal::shift_cursor(cursor_pos _dpos)
	{
		auto _cursor = this->get_cursor_pos();
		_cursor.x += _dpos.x;
		_cursor.y += _dpos.y;
		this->set_cursor_pos(_cursor);
	};


	void terminal::insert(const char* _cstr, size_t _len)
	{

	};

	void terminal::write(const char* _cstr, size_t _len)
	{
		if (!this->term_)
		{
			JTERM_NOTIFY_ERROR("terminal::write Invalid Terminal Handle", "Terminal Error");
			return;
		};
		if (_len == 0)
		{
			JTERM_NOTIFY_ERROR("terminal::write Invalid Terminal Handle", "Terminal Error");
			return;
		};

		auto& _term = *this->term_;

		size_t _writtenCount = 0;
		while (_writtenCount < _len)
		{
			DWORD _written = 0;
			if (!WriteFile(
				_term.bout,					// output handle
				_cstr + _writtenCount,		// string
				_len - _writtenCount,		// string length
				&_written,					// bytes written
				NULL))						// not overlapped
			{
				JTERM_NOTIFY_ERROR("terminal::write Failure To Write", "Terminal Error");
				return;
			};

			if (_written == 0)
			{
				JTERM_NOTIFY_ERROR("terminal::write No characters written!", "Terminal Error");
				return;
			};

			_writtenCount += _written;
		};

	};

	void terminal::writeln(const char* _cstr, size_t _len)
	{
		this->write(_cstr, _len);

		auto& _term = *this->term_;

		CONSOLE_SCREEN_BUFFER_INFO _info;
		GetConsoleScreenBufferInfo(_term.bout, &_info);
		
		const auto& _cursorPos = _info.dwCursorPosition;
		const auto& _windowSize = _info.dwSize;

		if (_cursorPos.Y >= (_windowSize.Y - 1))
		{
			const auto _scrollCount = _cursorPos.Y - (_windowSize.Y - 1);
			this->scroll(_scrollCount);
		};

		// Where to move the cursor to
		auto _newCursorPos = _cursorPos;
		++_newCursorPos.Y;
		_newCursorPos.X = 0;
		if (!SetConsoleCursorPosition(_term.bout, _newCursorPos))
		{
			JTERM_NOTIFY_ERROR("terminal::writeln Failed to move cursor!", "Terminal Error");
			return;
		};

	};

	void terminal::scroll(int _count)
	{
		auto& _term = *this->term_;
		scroll_screen_buffer(_term.bout, 1, static_cast<SHORT>(_count));
	};

};

#endif