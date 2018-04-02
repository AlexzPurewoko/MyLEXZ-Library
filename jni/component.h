#ifndef _COMPONENT_H_
#define _COMPONENT_H_
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <math.h>
#include <ctype.h>
#ifdef _DEF_TYPES_
 #define INT 0x000008
 #define STR 0x000009
 #define CHR 0x00000a
 #define DOUBLE 0x00000b
 //#define LDB 0x00000c
 #define LONG 0x00000d
 #define BOOL 0x00000e
#endif /* _DEF_TYPES_ */
/**
 * Mode for encrypting and decrypting
 */
static enum {
	ENC,
	DEC
}MODE;
/*
* Convert characters into lowerCase
*/
char toLower(int c);
/************/
/*
* cf - >  Comparing character c and t
*/
int cf(const char c, const char t);
/**
 * This function is used to compare 2 strings 
 * how to use
 * 		cmp(string1, string2)
 * this method will return 1 (true) if string1 and string2
 * have a same character elements and have a same length
 * and will return 0 vice-versa
 */
int cmp(const char s[],  const char t[]);
/**
 * reverse(String_to_reverse);
 * this function is used to reverse string values
 * and return back into its array
 * without returning back 
 */
void reverse(char s[]);
/**
 * This function is used to check the file is exists or not
 * how to use
 * checkExists(filename<string>);
 * This method will return 0 if no error occurs if searching the file
 * and the file is exists in directory
 * return 1 if the file isn't exists and the error is occurs if searching the file
 */
int checkExists(const char *filename);
extern long double atollf(const char *num);
extern double atof(const char *s);
#endif
