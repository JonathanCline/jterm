#pragma once
#ifndef JTERM_CPP_HPP
#define JTERM_CPP_HPP

// Where to place the copywrite notice once this is automated
#pragma jterm_copywrite_notice

/*
	C++ interface for jterm
*/

#define _JTERM_CPP_

// Reaffirm that this is a C++ instance
#ifndef __cplusplus
#error "jterm c++ interface header was included but __cplusplus is not defined!"
#endif


#include <string_view>


/**
 * @brief jterm root namespace
*/
namespace jterm
{

	/**
	 * @brief Terminal instance data
	*/
	struct terminal_instance;



	terminal_instance* new_terminal();


	void delete_terminal(terminal_instance*& _term);




	// Writes a string to the terminal
	void write(terminal_instance& _term, std::string_view _str, int _x, int _y);



	void refresh(terminal_instance& _term);



	void set_foreground_color(terminal_instance& _term, uint8_t r, uint8_t g, uint8_t b);
	void set_background_color(terminal_instance& _term, uint8_t r, uint8_t g, uint8_t b);

	
	void put(terminal_instance& _term, char _char, int _x, int _y);



	using text_callback_fn = void(*)(void* _user, char _text);
	
	// @param _userParam Passed to the callback function on invocation
	void set_callback(terminal_instance& _term, text_callback_fn _callback, void* _userParam);





};

#endif