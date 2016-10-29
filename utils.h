#ifndef utils_h_included
#define utils_h_included

#define ALPHA_MIN        'a'
#define ALPHA_MAX        'z'
#define VALID_OPERATORS  "^/*-+"
#define LEFT_PAREN       '('
#define RIGHT_PAREN      ')'

typedef enum {
   INFIX_RULES = 0,
   RPN_RULES,
   RULES_COUNT
} validation_t;

int isLowerCaseLetter(char letter);
int isAllowedOperator(char letter);
int checkMatchingParenthesis(const char *str);
int checkValidChars(const char *str, validation_t validation_rule);


#endif /* utils_h_included */
