#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <libgen.h>

#include "debug.h"
#include "limit.h"

//typedef enum { False, True} Bool; ///< we use C99 <stdbool.h>

/**
 *	Check file exist or not.
 *		man 2 access or follow http://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c-cross-platform
 *
 *	@param fname file name
 *
 *	@return true
 *	@return false
 */
bool fexist( char* fname);

/**
 *	Check the path is directory or not.
 *
 *	@param dname directory path
 *
 *	@return true
 *	@return false
 */
bool isdir( char* dname);

/**
 *	Make a  directory with specify Permission
 *
 *	@param dname directory path
 *
 *	@return true
 */
bool mkd( char* dname, mode_t mode);

/**
 *	String concatenate with directory and mutiple file
 *
 *	@param dir 				directory path
 *	@param files 			input file path arrary
 *	@param n 					file number
 *	@param out_files 	output file path arrary
 */
void apen_dir_file( char* dir, char** files, int n, char** out_files);///< i fail with strdup

/**
 * String arrary append specify suffix
 *
 *	@param in 			input string arrary
 *	@param suffix 	suffix
 *	@param n 				arrary element number
 */
void apen_suffix( char** in, int n, char* suffix);

char* ret_cwd( void);

char* ret_realpath( char* path);

#endif///< #ifndef FILE_OPS_H
