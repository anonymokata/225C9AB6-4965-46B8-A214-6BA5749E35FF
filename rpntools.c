/* utility functions */

#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "utils.h"
#include "rpntools.h"
#include "errors.h"

#define MATCH 0
#define dumpstack()     for (i=0; i<strlen(string); i++) { printf("stack[%d]: %s\n", i, stack[i]); }

/* run checks on input string before we use it */
void checkSanity(const char *str, validation_t validation_rule)
{
   if (checkValidChars(str, validation_rule) != VALID_CHARPOS) {
      setError(ERR_INVALID_CHARACTER);
   }

   if (checkOperatorBalance(str) == NOK) {
      setError(ERR_UNBALANCED_EXPRESSION);
   }

   /* only for infix, check parenthesis matching */
   if (validation_rule == INFIX_RULES) {
      if (checkMatchingParenthesis(str) != OK) {
         setError(ERR_PARENTHESIS_UNBALANCED);
      }
   }
}

/* given a string like "ab+c*d^" generate --> ((a+b)*c)^d */
const char* RPNtoInfix(const char *string)
{
   int i, pos;
   char op, letter;
   char last[SMBUFFER];
   char first[SMBUFFER];
   char stack[STACKSIZE][SMBUFFER];
   static char result[SMBUFFER];
   pos = 0;

   resetErrors();  /* reset errors at start of run */
   strcpy(result, "");  /* initialize result with empty string */
   checkSanity(string, RPN_RULES);  /* check sanity of input string */

   /* if the string is larger than stacksize, or if the string would overflow
    * the stack given the startpos, set error and return immediately */
   if (strlen(string) > STACKSIZE) {
      setError(ERR_STRING_TOO_LONG);
   }

   if (getErrors()) {
      return "";  /* return empty string on error */
   }

   /* look through every letter */
   for (i=0; i<strlen(string); i++) {
      letter = string[i];
      if (isLowerCaseLetter(letter)) {
         if (pos < STACKSIZE) {
            pushchar(letter, stack); /* insert letters into stack */
         }
         else {
            setError(ERR_STACK_OVERFLOW);
            break;
         }
      }
      else if (isAllowedOperator(letter)) {
         op = letter;
         if (pos > 1) {
            popstr(last, stack);
            popstr(first, stack);
         }
         else {
            setError(ERR_UNBALANCED_EXPRESSION);
            break;
         }
         /* re-arrange the values into infix notation, wrap with parenthesis, and put into stack again */
         sprintf(stack[pos++], "(%s%c%s)", first, op, last);
      }
   }

   if (!getErrors()) {
      /* the last string on the stack is the result */
      popstr(result, stack);
   }

   return result;
}



/* getRPN will convert an equation into an RPN expression.  The expression is passed as an array of strings.
 * For example:
 * ["a", "+", "b"] will return "ab+"
 * ["a", "+", "bc*"] will return "abc*+"
 * ["ab*", "*", "dc*"] will return "ab*dc**" */
const char* getRPN(char array[][SMBUFFER], int arraylen)
{
   int let, i, left, right, lastop;
   char op;
   static char result[SMBUFFER];  /* will hold the final result */ 
   i=0, left=0, right=0, lastop=0;

   strcpy(result, "");  /* init with empty */

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
            left = i-1;  /* the value behind the operator */
            right = i+1;  /* the value in front of the operator */

            while (isEmptyStr(array[left])) {
               left-=1;
            }

            while (isEmptyStr(array[right])) {
               right+=1;
            }

            /* if there is an operator on the left or the right, something is wrong, we are done */
            /* if left position is <= 0, we are done */
            if (     ( strlen(array[left])==1 && isAllowedOperator(array[left][0]) )
                  || ( strlen(array[right])==1 && isAllowedOperator(array[right][0]) ) 
                  || ( left < 0)  
               ) 
            {
               setError(ERR_UNBALANCED_EXPRESSION);
               break;
            }

            /* create RPN notation */
            sprintf(array[i], "%s%s%c", array[left], array[right], op);

            /* make empty strings in left and right as we don't need them now */
            array[left][0] = 0;
            array[right][0] = 0;

            /* remember this position */
            lastop = i;
         }
      }
   }

   /* copy result into static storage */
   if (!getErrors()) 
      strcpy(result, array[lastop]);

   return result;
}


/* given an infix string, "(a+b)*(c^d)", this function should parse it appropriately
 * and return RPN notation, i.e. (ab+cd^*)  */
const char* InfixtoRPN(const char *string)
{
   int pos=0;
   int left=-1;
   int right=-1;
   int i;
   char stack[STACKSIZE][SMBUFFER];
   char equation[SMBUFFER];
   static char result[SMBUFFER];

   resetErrors();                     /* reset errors at start of run */
   strcpy(result, "");                /* initialize result with empty string */
   checkSanity(string, INFIX_RULES);  /* check sanity of input string */

   /* if the string is larger than stacksize, or if the string would overflow
    * the stack given the starpos, set error and return immediately */
   if (strlen(string) > STACKSIZE) {
      setError(ERR_STRING_TOO_LONG);
   }

   /* if there are any errors, we are done right now */
   if (getErrors()) {
      return "";  /* return empty string on error */
   }

   setStackWithString(stack, 0, string);

   //dumpstack();  /* for debugging */

   while (1) {
      if (strcmp(stack[pos], "(")==MATCH) {
         left = pos;
      }
      if (strcmp(stack[pos], ")")==MATCH) {
         right = pos;
      }

      /* found two parenthesis */
      if ((left != -1) && (right != -1)) {
         strcpy(equation, getRPN(&stack[left+1], right-left));

         if (getErrors() || strlen(equation) == 0) {
            break;
         }

         for (i=left; i<right+1; i++) {
            strcpy(stack[i], "");
         }
         strcpy(stack[left], equation);
         pos = 0;
         left = -1;
         right = -1;
         continue;
      }

      if (pos < strlen(string)-1) {
         pos += 1;
      }
      else {
         getRPN(stack, pos);
         break;
      }
   }

   /* if there are any errors, we are done right now */
   if (getErrors()) {
      return "";  /* return empty string on error */
   } 

   /* we are done, somewhere in the stack is our string */
   for (i=0; i<strlen(string); i++) {
      if (strlen(stack[i]) > 0)
         break;
   }

   /* copy to static mem */
   strcpy(result, stack[i]);

   return result;
}




