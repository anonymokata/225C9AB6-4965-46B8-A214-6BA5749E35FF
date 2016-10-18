API description

How to build

Assumptions

Description of the exercise



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

