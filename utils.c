#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "utils.h"
#include "errors.h"

/* determine if this character is a lower case letter */
/* TODO: consider replacing with macro */
int isLowerCaseLetter(char letter) 
{
   int status = NOK;

   /* if the letter is within spec, do nothing, else report position of error */
   if ((letter >= ALPHA_MIN) && (letter <= ALPHA_MAX)) {
      status = OK;
   }

   return status;
}

/* determine if this character is an allowed operator */
/* TODO: consider replacing with macro, simplify? */
int isAllowedOperator(char letter)
{
   int status = NOK;
   int i;

   for (i=0; i<strlen(VALID_OPERATORS); i++) {
      if (letter == VALID_OPERATORS[i]) {
         status = OK;
         break;
      }
   }

   return status;
}

/* check if parenthesis are matching, left parens should equal right parens */
/* return OK if parenthesis are ok, NOK if something wrong */
int checkMatchingParenthesis(const char *str) 
{
   int i, count, result;

   count = 0;
   result = OK;

   /* increment for left paren, decrement for right paren */
   for (i=0; i<strlen(str); i++) {
      if (str[i] == '(')
         count += 1;
      if (str[i] == ')')
         count -= 1;
      if (count < 0)  /* unbalanced */
         break;
   }

   /* if parenthesis are balanced, count will equal zero */
   if (count != 0) 
      result = NOK;

   return result;
}

/* check for valid characters in string per specification */
/* pass string '(a+b)*c' or 'ab+' */
/* inifixrules */
/* return character position of failure */
int checkValidChars(const char *str, validation_t validation_rule)
{
   int i;
   int errorposition=VALID_CHARPOS;
   char letter;

   /* look through every letter */
   for (i=0; i<strlen(str); i++) {
      letter = str[i];
      /* if the letter is within spec, do nothing, else report position of error */
      if ((isLowerCaseLetter(letter)==OK) || (isAllowedOperator(letter)==OK)) {
         continue;
      }
      else if ((validation_rule == INFIX_RULES) && ((letter == LEFT_PAREN) || (letter == RIGHT_PAREN))) {
         continue;
      }
      else {
         errorposition = i;
         break;
      }
   }

   return errorposition;
}



