#pragma once
#ifndef JTERM_CPP_HPP
#define JTERM_CPP_HPP

// Where to place the copywrite notice once this is automated
#pragma jterm_copywrite_notice





/*
	C++ interface for jterm
*/

#include <jterm/common/jterm_common.h>

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
	using terminal = ::jterm_Terminal;


	/**
	 * @brief Creates a new terminal
	*/
	terminal* new_terminal();

	/**
	 * @brief Deletes a terminal
	 * @param _term Terminal instance pointer, will be set to null after deletion
	*/
	void delete_terminal(terminal*& _term);


	/**
	 * @brief Creates a new terminal
	*/
	jterm_Terminal* new_terminal();

	/**
	 * @brief Deletes a terminal
	 * @param _term Terminal instance pointer, will be set to null after deletion
	*/
	void delete_terminal(jterm_Terminal*& _term);



	void get_color(const jterm_Terminal& _term, jterm_Pos _x, jterm_Pos _y, jterm_Color* _foreground, jterm_Color* _background);
	void set_color(jterm_Terminal& _term, jterm_Pos _x, jterm_Pos _y, jterm_Color _foreground, jterm_Color _background);

	void fill_color(jterm_Terminal& _term, jterm_Color _fg, jterm_Color _bg, jterm_Pos _x, jterm_Pos _y, jterm_Pos _w, jterm_Pos _h);




	void set(jterm_Terminal& _term, jterm_Char _codepoint, jterm_Pos _x, jterm_Pos _y);



};

#endif