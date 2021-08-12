#include <version>


#include <iostream>
#include <format>
#include <string>
#include <string_view>
#include <concepts>



// Brief feature testing section

#ifndef __cpp_lib_format
#error "Missing required feature : __cpp_lib_format"
#endif
#ifndef __cpp_concepts
#error "Missing required feature : __cpp_concepts"
#endif
#ifndef __cpp_lib_string_view
#error "Missing required feature : __cpp_lib_string_view"
#endif



namespace jterm::gen
{
	// Specialize to implement custom formatters
	template <typename T, typename CharT>
	struct formatter;

	template <typename T>
	concept cx_custom_formattable = requires(formatter<T, char> _fmter, const T & _val, std::format_context & _ctx)
	{
		{ _fmter.format(_val, _ctx) } -> std::same_as<std::string>;
	};
};

namespace std
{
	// Custom formatter redirection
	template <::jterm::gen::cx_custom_formattable T, typename CharT>
	struct formatter<T, CharT> : public ::std::formatter<::std::basic_string_view<CharT>, CharT>
	{
	private:
		using parent_type = ::std::formatter<::std::basic_string_view<CharT>, CharT>;

	public:

		auto parse(auto& _ctx)
		{
			return _ctx.end();
		};
		auto format(const T& _value, auto& _ctx)
		{
			const auto _formatted = this->custom_.format(_value, _ctx);
			return parent_type::format(_formatted,  _ctx);
		};

		using parent_type::parent_type;

	private:
		::jterm::gen::formatter<T, CharT> custom_{};
	};

	template <size_t N, typename CharT>
	struct formatter<CharT[N], CharT> : public ::std::formatter<::std::basic_string_view<CharT>, CharT>
	{
	private:
		using parent_type = ::std::formatter<::std::basic_string_view<CharT>, CharT>;

		using value_type = CharT[N];

	public:

		auto parse(auto& _ctx)
		{
			return _ctx.end();
		};
		auto format(const value_type& _value, auto& _ctx)
		{
			return parent_type::format(_value, _ctx);
		};

		using parent_type::parent_type;

	private:
	};


};

namespace jterm::gen
{
	namespace impl
	{
		template <typename T>
		struct is_char_array : std::false_type {};

		template <size_t N>
		struct is_char_array<const char[N]> : std::true_type {};

		template <size_t N>
		struct is_char_array<char[N]> : std::true_type {};
	};


	template <typename T>
	concept cx_formattable = requires(std::string_view _sv, T a)
	{
		std::formatter<T>{};
		{ std::format(_sv, a) } -> std::same_as<std::string>;
	};

	struct make_format_t
	{
		struct format_t
		{
			std::string_view fmt;
		};
		constexpr format_t operator()(std::string_view _fmt) const noexcept
		{
			return format_t{ _fmt };
		};
	};
	constexpr inline make_format_t make_format{};



	namespace fmt_literals
	{
		constexpr inline make_format_t::format_t operator""_fmt(const char* _str, size_t _len) noexcept
		{
			return make_format(std::string_view{ _str, _len });
		};
	};
	using namespace fmt_literals;




	// Maybe...
	struct fmtstream
	{
	private:
		constexpr static size_t max_args_v = 64;
		constexpr static auto default_fmt_v = []()
		{
			std::array<char, max_args_v * 2> _out{};
			for (auto it = _out.begin(); it != _out.end();)
			{
				*it = '{';
				++it;
				*it = '}';
				++it;
			};
			return _out;
		}();

	public:




		template <cx_formattable... Ts>
		fmtstream& write(make_format_t::format_t _format, const Ts&... _values)
		{
			const auto _fmtResult = std::format(_format.fmt, _values...);
			this->ostr_.write(_fmtResult.data(), _fmtResult.size());
			return *this;
		};

		template <cx_formattable... Ts> requires (sizeof...(Ts) <= max_args_v)
		fmtstream& write(const Ts&... _values)
		{
			const std::string_view _fmt{ this->default_fmt_v.data(), sizeof...(Ts) * 2 };
			return this->write(make_format(_fmt), _values...);
		};







		template <cx_formattable... Ts>
		fmtstream& writeln(make_format_t::format_t _format, const Ts&... _values)
		{
			const auto _fmtResult = std::format(_format.fmt, _values...);
			this->ostr_.write(_fmtResult.data(), _fmtResult.size());
			this->ostr_.put('\n');
			return *this;
		};
		
		template <cx_formattable... Ts> requires (sizeof...(Ts) <= max_args_v)
		fmtstream& writeln(const Ts&... _values)
		{
			const std::string_view _fmt{ this->default_fmt_v.data(), (sizeof...(Ts) + 1) * 2 };
			return this->write(make_format(_fmt), _values..., '\n');
		};



		template <cx_formattable... Ts>
		fmtstream& operator()(make_format_t::format_t _format, const Ts&... _values)
		{
			return this->write(_format, _values...);
		};

		template <cx_formattable... Ts> requires (sizeof...(Ts) <= max_args_v)
		fmtstream& operator()(const Ts&... _values)
		{
			return this->write(_values...);
		};



		fmtstream(std::streambuf* _buf) :
			ostr_{ _buf }
		{};

	private:
		std::ostream ostr_;
	};





};


extern inline jterm::gen::fmtstream cout{ std::cout.rdbuf() };
using namespace jterm::gen::fmt_literals;



int main(int _nargs, const char* _args[])
{
	
	if (_nargs == 1)
	{
		cout("{} is a rad dude\n"_fmt, "makin pancakes");

		cout("{} is a rad dude\n", "slime\n");

		cout(jterm::gen::make_format("{} is a rad dude\n"), "your mom");
	};


	return 0;
};
