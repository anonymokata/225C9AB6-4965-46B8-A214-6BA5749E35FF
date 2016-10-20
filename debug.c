#include <stdio.h>
#include "defs.h"
#include "rpntools.h"

int main(void) {
   printf("\n");
   printf("%s\n", RPNtoInfix("ab+"));
   printf("%s\n", RPNtoInfix("ab+c*d*"));
}
