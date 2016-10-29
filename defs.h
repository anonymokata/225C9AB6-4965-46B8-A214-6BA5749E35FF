#ifndef defs_h_included
#define defs_h_included
/* general/global definitions */

#define NOK  0
#define OK   1

#define TRUE 1
#define FALSE 0

#define VALID_CHARPOS -1

#define LGBUFFER  1024
#define SMBUFFER  256
#define STACKSIZE 64

/* macros for push/pop strings onto 'stack' */
#define pushchar(chr, stack)    sprintf(stack[pos++], "%c", chr)
#define pushstr(str, stack)     sprintf(stack[pos++], "%s", str) 
#define popstr(dest, stack)     sprintf(dest, "%s", stack[--pos])

/* macro to determine if string is a single letter */
#define isOneChar(string)       (strlen(string)==1)
#define isEmptyStr(string)      (strlen(string)==0)

#define push(sp, n) (*((sp)++) = (n))
#define pop(sp) (*--(sp))

#endif
