#ifndef util_h_included
#define util_h_included

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

typedef enum {
   ERR_PARENTHESIS_UNBALANCED = 0,
   ERR_INVALID_OPERATOR,
   ERR_INVALID_CHARACTER,
   ERR_STRING_TOO_LONG,
   ERR_TOO_MANY_OPERATORS,
   ERR_TOO_MANY_OPERANDS,
   ERR_CODES_COUNT
} errorcodes_t;

int isLowerCaseLetter(char letter);
int isAllowedOperator(char letter);
int checkMatchingParenthesis(const char *str);
int checkValidChars(const char *str, validation_t validation_rule);
int checkSanity(const char *str, validation_t validation_rule);
const char* RPNtoInfix(const char *str);


#endif
