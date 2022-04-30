#ifndef COMPILE_H_INCLUDED
#define COMPILE_H_INCLUDED


#include <stdio.h>
#include <string.h>
#include "great_grammar.h"
#include "lexo_parser.h"
#include "..\hamlet\d_hamlet_functions.h"
#include "..\dictionary\dict.h"
#include "to_asm.h"


void assembly (const char* file_name);


#endif // COMPILE_H_INCLUDED
