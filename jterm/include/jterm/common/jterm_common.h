#pragma once
#ifndef JTERM_COMMON_H
#define JTERM_COMMON_H

// Where to place the copywrite notice once this is automated
#pragma jterm_copywrite_notice



/*
	Defines types / type aliases shared between the C and C++ API headers
*/

#define _JTERM_COMMON_

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief Screen position type alias
	*/
	typedef short jterm_Pos;

	/**
	 * @brief Holds an RGBA color value
	*/
	typedef struct
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	} jterm_Color;

	/**
	 * @brief Terminal instance data
	*/
	typedef struct jterm_Terminal;



	typedef char jterm_Char;




#ifdef __cplusplus
};
#endif

#endif