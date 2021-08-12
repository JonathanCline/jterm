#pragma once
#ifndef JTERM_C_H
#define JTERM_C_H

// Where to place the copywrite notice once this is automated
#pragma jterm_copywrite_notice





/*
	C interface for jterm
*/

#define _JTERM_C_

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief Terminal instance data
	*/
	struct jterm_Terminal;

	/**
	 * @brief Creates a new terminal
	*/
	struct jterm_Terminal* jterm_new_terminal();

	/**
	 * @brief Deletes a terminal
	 * @param _term Terminal instance pointer, will be set to null after deletion
	*/
	void jterm_delete_terminal(struct jterm_Terminal** _term);








#ifdef __cplusplus
};
#endif

#endif