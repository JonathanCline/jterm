#pragma once
#ifndef JTERM_H
#define JTERM_H

// Where to place the copywrite notice once this is automated
#pragma jterm_copywrite_notice


/*
	Main public interface header for jterm - includes the appropriate header file for C/C++ usage.
*/

#if defined(__cplusplus)
#define JTERM_CPP
#else
#define JTERM_C
#endif

#ifdef JTERM_CPP
#include <jterm/cpp/jterm.hpp>
#endif

#ifdef JTERM_C
#include <jterm/c/jterm.h>
#endif

#endif