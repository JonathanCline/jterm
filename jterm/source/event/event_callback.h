#pragma once

#include <jterm/cpp/jterm.hpp>

#include <jclib/concepts.h>

#include <concepts>

namespace jterm
{

	// Generic callback data storage
	template <typename FnT>
	struct callback
	{
	public:

		using callback_type = FnT;

		constexpr bool is_set() const noexcept
		{
			return this->cb_;
		};
		constexpr explicit operator bool() const noexcept
		{
			return this->is_set();
		};



		template <typename... Ts>
		requires std::invocable<callback_type, void*, Ts&&...>
		constexpr auto invoke(Ts&&... _vals) const
		{
			JCLIB_ASSERT(this->is_set());
			return std::invoke(this->cb_, this->uparam_, std::forward<Ts>(_vals)...);
		};

		template <typename... Ts>
		requires std::invocable<callback_type, void*, Ts&&...>
		constexpr auto operator()(Ts&&... _vals) const
		{
			return this->invoke(std::forward<Ts>(_vals)...);
		};



		constexpr callback() noexcept = default;
		constexpr callback(callback_type _callback, void* _uparam) noexcept :
			cb_{ _callback }, uparam_{ _uparam }
		{};

	private:
		callback_type cb_{};
		void* uparam_ = nullptr;
	};


	struct event_callbacks
	{
		callback<text_callback_fn> on_text;
	};





};
