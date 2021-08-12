#pragma once

#include "os/os.hpp"

#include <string_view>

namespace jterm
{
	/**
	 * @brief Terminal window + instance data
	*/
	struct terminal_instance
	{
		using allow_reuse_t = os_terminal::allow_reuse_t;
		constexpr static allow_reuse_t allow_reuse{};

		terminal_instance() = default;
		terminal_instance(allow_reuse_t) :
			term_{ allow_reuse }
		{};

		os_terminal term_;
	};

	/**
	 * @brief Creates a new terminal
	*/
	terminal_instance* new_terminal();

	/**
	 * @brief Deletes a terminal
	 * @param _term Terminal instance pointer, will be set to null after deletion
	*/
	void delete_terminal(terminal_instance*& _term);



	// Writes a string to the terminal
	void write(terminal_instance& _term, std::string_view _str);

};
