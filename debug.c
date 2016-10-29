#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "rpntools.h"

int main(void) {
   int pos;
   char stack[STACKSIZE][SMBUFFER];

   printf("\n");
   //printf("%s\n", RPNtoInfix("ab+"));
   //printf("%s\n", RPNtoInfix("ab+c*d*"));
   //printf("%s\n", RPNtoInfix("ab+c*d^"));
   printf("%s\n", RPNtoInfix("ab+"));

   printf("\n");
   pushstr("a", stack);
   pushstr("+", stack);
   pushstr("b", stack);
   pushstr("*", stack);
   pushstr("c", stack);
   printf("getRPN:\n");

   printf("%s", getRPN(stack, pos));
   return 0;
}
