/* utility functions */

#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "rpntools.h"

/* global var to store error flags */
unsigned int ERROR_FLAGS = 0;

/* set a bit representing an error */
/* a macro for this would be more efficient but not sure if i can use 
 * the testing framework to test macros? */
void setErrorFlag(flagnum) 
{
   ERROR_FLAGS |= (1<<flagnum);
}

/* get a bit representing an error */
/* a macro for this would be more efficient but not sure if i can use 
 * the testing framework to test it */
int getErrorFlag(flagnum) 
{
   return (ERROR_FLAGS & (1<<flagnum) ? 1 : 0);
}

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

   if (result == NOK)
      setErrorFlag(ERR_PARENTHESIS_UNBALANCED);

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

   if (errorposition != VALID_CHARPOS)
      setErrorFlag(ERR_INVALID_CHARACTER);

   return errorposition;
}

/* run checks on input string before we use it */
int checkSanity(const char *str, validation_t validation_rule)
{
   int status = NOK;
   if (    (checkValidChars(str, validation_rule) == VALID_CHARPOS)
        && (checkMatchingParenthesis(str) == OK) ) {
      status = OK;
   }

   return status;
}

/* given a string like "ab+c*d^" generate --> ((a+b)*c)^d */
const char* RPNtoInfix(const char *str)
{
   int i, pos;
   char op, letter;
   char last[SMBUFFER];
   char first[SMBUFFER];
   char stack[STACKSIZE][SMBUFFER];
   static char result[SMBUFFER];

   pos = 0;
   resetErrors();

   strcpy(result, "");  /* initialize with empty string */

   if (checkSanity(str, RPN_RULES) != OK) {
      return result;  /* TODO report error */
   }

   /* look through every letter */
   for (i=0; i<strlen(str); i++) {
      letter = str[i];
      if (isLowerCaseLetter(letter)) {
         if (pos < STACKSIZE) {
            pushchar(letter); /* insert letters into stack */
         }
         else {
            setErrorFlag(ERR_STACK_OVERFLOW);  /* set error too large */
            break;
         }
      }
      else if (isAllowedOperator(letter)) {
         op = letter;
         if (pos > 1) {
            popstr(last);
            popstr(first);
         }
         else {
            setErrorFlag(ERR_UNBALANCED_EXPRESSION);
            break;
         }
         /* re-arrange the values into infix notation, wrap with parenthesis, and put into stack again */
         /* TODO: protect against string that is too long */
         sprintf(stack[pos++], "(%s%c%s)", first, op, last);
      }
   }

   if (!ErrorsSet()) {
      /* the last string on the stack is the result */
      popstr(result);
   }

   return result;
}

/* getRPN will convert an equation into an RPN expression.  The expression is passed as an array of strings.
 * For example:
 * ["a", "+", "b"] will return "ab+"
 * ["a", "+", "bc*"] will return "abc*+"
 * ["ab*", "*", "dc*"] will return "ab*dc**" */
const char* getRPN(char array[STACKSIZE][SMBUFFER], int arraylen)
{
   int let, i, j, k, lastop;
   char op;
   static char result[SMBUFFER];  /* will hold the final result */

   i=0, j=0, k=0, lastop=0;

   if (arraylen == 0)  /* nothing to do? */
      return "";

   /* now loop through the entire array (essentially letter by letter, left to right) in order
    * of operator precedence.  Each operator should have an operand on the left and an operand on the right.
    * ['', '', 'LEFT', 'OPERATOR', 'RIGHT', '', '']
    * ['LEFT', '', '', 'OPERATOR', '', '', '', 'RIGHT']  <-- also valid 
    * When an operator is found, the value to the left and right are taken, and converted to RPN with 
    * the following method:  [LEFT][RIGHT][OPERATOR], so a+b becomes ab+.
    * once we have looped through all operators, there should be nothing left to do.  */
   for (let=0; let<strlen(VALID_OPERATORS); let++) {
      op = VALID_OPERATORS[let];  /* go through operators in order of precedence */
      for (i=0; i<arraylen; i++) {
         /* we found the operator */
         if (isOneChar(array[i]) && (array[i][0] == op)) {
            j = i-1;  /* the value behind the operator */
            k = i+1;  /* the value in front of the operator */

            while (isEmptyStr(array[j])) {
               j-=1;
            }

            while (isEmptyStr(array[k])) {
               k+=1;
            }

            /* create RPN notation */
            sprintf(array[i], "%s%s%c", array[j], array[k], op);

            /* delete the strings in j and k as we don't need them now */
            strcpy(array[j], "");
            strcpy(array[k], "");

            /* remember this position */
            lastop = i;
         }
      }
   }

   printf("%s\n", array[lastop]);

   return array[lastop];
}

