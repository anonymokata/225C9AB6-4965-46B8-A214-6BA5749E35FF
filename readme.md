API description
   From rpntools.h:
      * const char* RPNtoInfix(const char *str);
      Pass an equation in RPN notation, the return will contain an equivalent equation in Infix notation.  An empty string is returned on error.  
      Use the functions in errors.h to interpret errors.
      
      * const char* InfixtoRPN(const char *string);
      Pass an equation in Infix notation, the return will contain an equivalen equation in RPN notation.  An epmpty string is returned on error.
      Use the functions in errors.h to interpret errors.

   From errors.h:
      * errorflags_t
      The possible error flags that can be stored

      * int getErrors(void);
      Return an int which will contain all error flags stored

      * int getError(errorflags_t errorflag);
      Determine if one particular error was stored

How to build:
   * libcheck must be installed an functioning
   * clone git repository
   * from the project root, just type make, which will build the project and run the tests
   * make clean will clean the project

Assumptions
   * Operators will only consist of the following:  ^/*-+
   * Operands will only consist of lower case letters.
   * Parenthesis are allowed (and expected) when the user provides infix notation.
   * All other characters (including spaces) are unexpected, and therefore not allowed.
   * Unbalanced parenthesis are not allowed.
   * Strings have a maximum length.  If the user enters a string that exceeds any boundary, an empty string is returned and an error is generated.
   This was done to avoid dynamic memory allocation.  This approach will never leak memory, but it also uses memory inefficiently, and imposes
   a limit on the length of the input expression.  Limits such as this are not unreasonable; in most languages there are limits on input expression
   length (Linux commandline, Matlab commandline, etc.).

Description of the exercise
   Create functions to convert mathematical expressions to and from Infix/RPN notation.

Strategy used to solve the problem

   1. First look at the input/output requirements:
   operands: lowercase alphabet
   operators: ^/*-+
   parenthesis: only for Infix notation and invalid for RPN notation

   So the first thing that enters my mind is to make sure that the input string makes sense.  Some kind of
   sanity-check function to check if the input string makes sense.  Types of invalid input:

   Inifix type:
   3+3
   A+B
   FOO_*
   +3

   RPN type:
   (A+B)
   *AB

   In line with the above, it also makes sense to check if our opening and closing parenthesis are sane.

   Invalid:
   ((a+b)
   (((a+b)*(c+d))))

   So the first step is going to be input parsing before we get to the fun stuff.

   2.  Once we know we have the right characters and symbols, what about syntax?  
   ++ab doesn't make sense, and neither does (a+bb) or (a+b+)
   Operator/operand balance, and parenthesis balance is also checked during the input processing.

   3.  Conversion of RPN to Infix notation.
   If you've ever watched someone use the classic HP48 calculator, you witnessed RPN in action.  The beauty
   of this notation, once you're used to it, is that you can solve an equation like 1+(3*5) without the
   the additional steps of adding parenthesis, which would be necessary on any of the TI calculators.
   For 1+(3*5), an HP48 user would type: 1 [ENTER] 3 [ENTER] 5 [ENTER] * [ENTER] + [ENTER]
   As you use the HP48, you can watch the operands get entered onto a stack, while each press of an 
   operator performs the operation on the last two stack values.

   The strategy, then, for solving this problem is to use my own implementation of a string 'stack'. 
   If the input string is 135*+, the result needs to be (1+(3*5)).  With the problem statement, only
   lower case letters are allowed as operands, so in the same way:  abc*+ will become (a+(b*c)).

   The solution is to read the string left to right, and when encountering an operand, push it 
   onto the stack.  When encountering an operator, pop the last two values off of the stack and insert 
   the operator between them.  With the same example as before:
   push(a)
   push(b)
   push(c)
   [detected operator *]
   right = pop()  /* right becomes 'c' */
   left = pop()   /* left becomes 'b' */
   (left op right) becomes (b*c)
   push(b*c)
   [detected operator +]
   right = pop()  /* right becomes "(b*c)" */
   left = pop()   /* left becomes 'a' */
   (left op right) becomes (a+(b*c))

   Operator precedence isn't a problem with RPN, because the expression *already contains* the intended
   treatment.

   4.  Converion of Infix to RPN notation.
   This becomes more difficult, as we have to deal with operator precedence (for example, a+b*c^f),
   where c^f is handled first, then b*(c^f), and finally (a+(b*(c^f))).
   in RPN notation, we arrive at: abcf^*+

   Let's first tackle the issue of operator precedence, and deal with parenthesis later.

   We know that operator precedence is handled in this order:  ^, /, *, -, +.

   Therefore, which given an expression like above, "a+b*c^f", I decided to scan the entire string
   looking for operators in order of precedence, and grouping operations accordingly.

   If we convert the string into a string array, where each item on the stack is a string.  We then
   start with this:
   ["a", "+", "b", "*", "c", "^", "f"]

   To handle this, we scan the array looking for operators in order of precedence.  Once an operator is
   discovered, we have a situation like this:  [LEFT] [OPERATOR] [RIGHT].

   If the value in [LEFT] or [RIGHT] is an empty string, we may need to look farther left or right for 
   a non-zero value.

   Now we can assemble a string in RPN notation: [LEFT][RIGHT][OPERATOR], and insert this string where
   the [OPERATOR] was.
   Clear string at position [LEFT]
   Clear string at position [RIGHT]

   The array now looks like this:
   ["a", "+", "b", "*", "", "cf^", ""]

   Repeat the above strategy for the next operator "/"
   (nothing to do)

   Repeat the above strategy for the next operator "*"
   ["a", "+", "", "bcf^*", "", "", ""]

   Repeat again for "-"
   (nothing to do)

   And again for "+"
   ["", "abcf^*+", "", "", "", "", ""]

   So we have a method for handling infix to RPN *without* parenthesis.

   5. Adding parenthesis added another challenge.  






