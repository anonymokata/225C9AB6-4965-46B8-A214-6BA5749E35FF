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
#define pushchar(chr) sprintf(stack[pos++], "%c", chr)
#define pushstr(str) sprintf(stack[pos++], "%s", str) 
#define popstr(dest) sprintf(dest, "%s", stack[--pos])

#define push(sp, n) (*((sp)++) = (n))
#define pop(sp) (*--(sp))

#endif
