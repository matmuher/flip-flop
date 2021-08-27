# HOW TO BUILD IT INTO BINARY FOR WINDOWS:

  1. install g++ compiler
  2. download files in directory, open cmd in this directory
  3. to build test file enter in console:

* g++ -c test.c
* g++ -c beer_equation.c
* g++ -o test beer_equation.o test.o


*This will create test.exe in the directory*

  4. to build 3_coefs_equation_sovler file enter in console:

* g++ -c main.c
* g++ -c beer_equation.c
* g++ -o 3_coefs_equation_sovler beer_equation.o main.o


*This will create 3_coefs_equation_sovler.exe in the directory*

## Launch it to solve your 3_coefs_equation!
