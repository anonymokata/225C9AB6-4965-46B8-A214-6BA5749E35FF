#ifndef errors_h_included
#define errors_h_included

typedef enum {
   ERR_PARENTHESIS_UNBALANCED = 0,
   ERR_INVALID_CHARACTER,
   ERR_STRING_TOO_LONG,
   ERR_UNBALANCED_EXPRESSION,
   ERR_STACK_OVERFLOW,
   ERR_CODES_COUNT  /* this should always be last */
} errorflags_t;

/* reset all errors */
void resetErrors(void);

/* set an error flag */
void setError(errorflags_t errorflag);

/* determine if an error flag is set */
int getError(errorflags_t errorflag);

/* return all error flags set */
int getErrors(void);


#endif /* errors_h_included */
