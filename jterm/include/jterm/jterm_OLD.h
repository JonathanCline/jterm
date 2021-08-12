#pragma once

#include <memory>
#include <string_view>

namespace jterm
{
	struct terminal_handle;

	terminal_handle* new_terminal();
	void delete_terminal(terminal_handle*& _handle);



	struct terminal_handle_deleter
	{
		void operator()(terminal_handle* _handle)
		{
			delete_terminal(_handle);
		};
	};

	struct cursor_pos
	{
		int16_t x;
		int16_t y;
	};

	struct terminal
	{
	private:
		using handle =  std::unique_ptr<terminal_handle, terminal_handle_deleter>;
	
	public:
		
		cursor_pos get_cursor_pos() const;
		void set_cursor_pos(cursor_pos _pos);

		void shift_cursor(cursor_pos _dpos);



		void insert(const char* _cstr, size_t _len);
		auto insert(std::string_view _sv)
		{
			return this->insert(_sv.data(), _sv.size());
		};

		void write(const char* _cstr, size_t _len);
		auto write(std::string_view _sv)
		{
			return this->write(_sv.data(), _sv.size());
		};

		void writeln(const char* _cstr, size_t _len);
		auto writeln(std::string_view _sv)
		{
			return this->writeln(_sv.data(), _sv.size());
		};



		void scroll(int _count);
		auto scroll()
		{
			return this->scroll(1);
		};




		terminal() :
			term_{ new_terminal() }
		{};

	private:
		handle term_;
		
	};




};
