#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "rpntools.h"
#include "errors.h"

int main(void) {
   int pos;
   char stack[STACKSIZE][SMBUFFER];

   printf("\n");
   //printf("%s\n", RPNtoInfix("ab+"));
   //printf("%s\n", RPNtoInfix("ab+c*d*"));
   //printf("%s\n", RPNtoInfix("ab+c*d^"));
   printf("%s\n", RPNtoInfix("ab+"));
   printf("%02X\n", getErrors());

   printf("%s\n", RPNtoInfix("(++)"));
   printf("%02X\n", getErrors());

   printf("\n");
   pushstr("a", stack);
   pushstr("+", stack);
   pushstr("b", stack);
   pushstr("*", stack);
   pushstr("c", stack);
   printf("getRPN: %s\n", getRPN(stack, pos));
   return 0;
}
