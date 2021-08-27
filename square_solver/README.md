# HOW TO BUILD IT INTO EXECUTABLE FILE (.EXE) FOR WINDOWS:
## test.exe
*This file will start unit tests for our that solves square equation
  1. install g++ compiler
  2. download files in directory, open cmd in this directory
  3. to build test file enter in console:

* g++ -c test.c
* g++ -c beer_equation.c
* g++ -o test beer_equation.o test.o


*After that will be created test.exe in this directory*
## 3_coefs_equation_solver.exe
*This file will start programm that solves square equation, using your input coefficients
  4. to build 3_coefs_equation_solver file enter in console:

* g++ -c main.c
* g++ -c beer_equation.c
* g++ -o 3_coefs_equation_solver beer_equation.o main.o


*After that will be created 3_coefs_equation_solver.exe in the directory*
// ссылль на документацию
// работа прогрммы скрин
## Launch it to solve your 3_coefs_equation!
