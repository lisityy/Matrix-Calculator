# Matrix-Calculator
Matrix calculator for addition, subtraction and multiplication operations.

### Input:
In the standard input there are two or more matrices separated by one row with the operation sign (+,-,*). The operations are evaluated according to their priority.
Each matrix has its size (n, m) on the first row first, followed by n rows always with m matrix values. The individual values are separated by whitespaces.

### Output:
The format of the output is the same, but it contains only one matrix.

### Example:
_input:_

4 3

0 4 -9

-9 6 -4

3 5 -2

-1 7 5

\*

3 4

-10 -9 -8 9

-4 0 -9 1

4 6 -9 5

\+

4 4

0 -9 3 -6

10 -9 8 -7

-1 0 5 1

3 2 -9 9

_output:_

4 4

-52 -63 48 -47

60 48 62 -102

-59 -39 -46 23

5 41 -109 32
