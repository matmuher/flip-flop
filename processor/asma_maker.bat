g++ -c asma.cpp asma_funks.cpp ..\hamlet\d_hamlet_functions.cpp ..\strings\string_functions.cpp ..\stack\stack.cpp ..\memory_free\elephant_calloc.cpp ..\square_solver\beer_equation.cpp 2> err_txt

g++ asma.o asma_funks.o d_hamlet_functions.o string_functions.o stack.o elephant_calloc.o beer_equation.o -o asma 2>> err_txt

