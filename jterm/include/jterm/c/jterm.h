#pragma once
#ifndef JTERM_C_H
#define JTERM_C_H

// Where to place the copywrite notice once this is automated
#pragma jterm_copywrite_notice



/*
	C interface for jterm
*/


#include <jterm/common/jterm_common.h>

#define _JTERM_C_



#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief Creates a new terminal
	*/
	jterm_Terminal* jterm_new_terminal();

	/**
	 * @brief Deletes a terminal
	 * @param _term Terminal instance pointer, will be set to null after deletion
	*/
	void jterm_delete_terminal(jterm_Terminal** _term);


	/*
	
		
	
	*/



	void jterm_get_color(const jterm_Terminal* _term, jterm_Pos x, jterm_Pos y, jterm_Color* _foreground, jterm_Color* _background);

	void jterm_set_color(jterm_Terminal* _term, jterm_Pos x, jterm_Pos y, jterm_Color _foreground, jterm_Color _background);




	void jterm_fill_color(jterm_Terminal* _term, jterm_Color _fg, jterm_Color _bg, jterm_Pos _x, jterm_Pos _y, jterm_Pos _w, jterm_Pos _h);

	

	void jterm_refresh(jterm_Terminal& _term);




	void jterm_set(jterm_Terminal* _term, jterm_Char _codepoint, jterm_Pos _x, jterm_Pos _y);





#ifdef __cplusplus
};
#endif

#endif