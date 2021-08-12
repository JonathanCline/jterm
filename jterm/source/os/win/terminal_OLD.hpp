#pragma once

/*
	Windows specific terminal type and methods
*/


#include "event/event_callback.h"



#include <jclib/config.h>

#include <string>
#include <format>
#include <vector>
#include <string_view>
#include <source_location>
#include <ranges>
#include <algorithm>

#include <Windows.h>

#undef min
#undef max

namespace jterm::os
{
	void notify_error(const char* _message, const char* _caption);
};



namespace jterm::os
{
	// Shorthand for current source location
	consteval inline auto here(std::source_location _sloc = std::source_location::current())
	{
		return _sloc;
	};



	inline std::string make_debug_message(const char* _what, std::source_location _sloc)
	{
		constexpr std::string_view _fmt = "{1:} @{3:}\n\tin \"{2:}\"\n\t\"{0:}\"";
		return std::format(_fmt, _what, _sloc.file_name(), _sloc.function_name(), _sloc.line());
	};

	inline void notify_debug(const char* _what, std::source_location _sloc)
	{
		const auto _errmsg = make_debug_message(_what, std::move(_sloc));
		notify_error(_errmsg.c_str(), "jterm error!");
	};




	struct terminal;



	struct terminal_options_state
	{
	public:
		~terminal_options_state() = default;
	private:
		friend terminal;
		terminal_options_state() = default;

		DWORD mode;
		WORD  colors;
	};

	struct terminal_io_state
	{
	public:
		~terminal_io_state() = default;
	private:
		friend terminal;

		terminal_io_state() = default;
		terminal_io_state(HANDLE _in, HANDLE _out, HANDLE _err) :
			in_{ _in }, out_{ _out }, err_{ _err }
		{};

		HANDLE in_;
		HANDLE out_;
		HANDLE err_;
	};



	struct color_rgb
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	constexpr inline bool operator==(color_rgb lhs, color_rgb rhs) noexcept
	{
		return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b);
	};




	using terminal_cell = char;




	struct terminal_buffer
	{
		using value_type = terminal_cell;


		using size_type = size_t;



		size_type width() const noexcept
		{
			return this->width_;
		};
		size_type height() const noexcept
		{
			return this->height_;
		};

		size_type size() const noexcept
		{
			return this->width() * this->height();
		};

		void resize(size_type _width, size_type _height)
		{
			this->width_ = _width;
			this->height_ = _height;
			this->data_.resize(_width * _height);
		};



		value_type* data() noexcept
		{
			return this->data_.data();
		};
		const value_type* data() const noexcept
		{
			return this->data_.data();
		};



		size_type to_index(size_type _x, size_type _y) const
		{
			return (_y * this->width()) + _x;
		};
		std::pair<size_type, size_type> to_position(size_type _idx) const
		{
			size_type _y = _idx;
			_y /= this->width();

			size_type _x = _idx;
			_x -= static_cast<size_type>( _y * this->width() );

			return {  };
		};




		value_type& at(size_type _idx)
		{
			return this->data_.at(_idx);
		};
		value_type& at(size_type _x, size_type _y)
		{
			JCLIB_ASSERT(_x < this->width() && _y < this->height());
			return this->at(this->to_index(_x, _y));
		};
		
		const value_type& at(size_type _idx) const
		{
			return this->data_.at(_idx);
		};
		const value_type& at(size_type _x, size_type _y) const
		{
			JCLIB_ASSERT(_x < this->width() && _y < this->height());
			return this->at(this->to_index(_x, _y));
		};





		terminal_buffer(size_t _width, size_t _height) :
			width_{ _width }, height_{ _height },
			data_(_width* _height)
		{};

		std::vector<value_type> data_{};
		size_t width_ = 0;
		size_t height_ = 0;
	};



	struct terminal
	{
	private:
		terminal_io_state get_std_handles();

		void init_io_state();

	public:

		// Tag type for allowing terminal to use the already open terminal
		struct allow_reuse_t { constexpr explicit allow_reuse_t() = default; };
		constexpr static allow_reuse_t allow_reuse{};




		
		void write(std::string_view _str, size_t _x, size_t _y);
		void write(std::string_view _str)
		{
			auto& _cursor = this->cursor_;
			const auto [x, y] = this->buffer_.to_position(_cursor);
			this->write(_str, x, y);
			_cursor += _str.size();
		};

		void put(char _char, size_t  _x, size_t  _y);


		terminal_options_state get_options() const;
		void set_options(terminal_options_state _opState);



		void set_foreground_color(terminal_cell::color_type _col);
		void set_background_color(terminal_cell::color_type _col);



		auto& callbacks() noexcept { return this->callbacks_; };
		const auto& callbacks() const noexcept { return this->callbacks_; };

		void get_input_events();






		terminal();

		// Allows terminal to reuse the already open terminal if this fails to create a new one
		terminal(allow_reuse_t);


		~terminal();

		terminal(const terminal&) = delete;
		terminal(terminal&& other) noexcept = delete;


	private:
		event_callbacks callbacks_{};
		std::vector<terminal_cell> cells_;
		
		terminal_io_state io_;
		CONSOLE_SCREEN_BUFFER_INFO buffer_info_;
	};




};