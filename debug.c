#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "rpntools.h"

int main(void) {
   char stack[STACKSIZE][SMBUFFER];

   printf("\n");
   //printf("%s\n", RPNtoInfix("ab+"));
   //printf("%s\n", RPNtoInfix("ab+c*d*"));
   //printf("%s\n", RPNtoInfix("ab+c*d^"));
   printf("%s\n", RPNtoInfix("a+b"));

   printf("\n");
   strcpy(stack[0], "a");
   strcpy(stack[1], "+");
   strcpy(stack[2], "b");
   strcpy(stack[3], "*");
   strcpy(stack[4], "c");
}
