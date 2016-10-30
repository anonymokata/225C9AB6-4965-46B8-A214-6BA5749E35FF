#ifndef utils_h_included
#define utils_h_included

#define ALPHA_MIN        'a'
#define ALPHA_MAX        'z'
#define VALID_OPERATORS  "^/*-+"
#define LEFT_PAREN       '('
#define RIGHT_PAREN      ')'

#define VALID_CHARPOS -1

#define pushchar(chr, stack)    sprintf(stack[pos++], "%c", chr)
#define pushstr(str, stack)     sprintf(stack[pos++], "%s", str) 
#define popstr(dest, stack)     sprintf(dest, "%s", stack[--pos])

/* macro to determine if string is a single letter */
#define isOneChar(string)       (strlen(string)==1)
#define isEmptyStr(string)      (strlen(string)==0)

typedef enum {
   INFIX_RULES = 0,
   RPN_RULES,
   RULES_COUNT
} validation_t;

int isLowerCaseLetter(char letter);
int isAllowedOperator(char letter);
int checkMatchingParenthesis(const char *str);
int checkValidChars(const char *str, validation_t validation_rule);
void setStackWithString(char stack[][SMBUFFER], int startpos, char * string);
int checkOperatorBalance(const char *str);

#endif /* utils_h_included */
