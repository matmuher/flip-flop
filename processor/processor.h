#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED


#include "..\strings\string_funk.h"
#include "..\stack\stack.h"
#include "..\hamlet\d_hamlet_functions.h"
#include "..\square_solver\beer_equation.h"


// Historical heritage
#if 0
    // Undefined behavior if both 0, or both 1
    #define NON_DISASM 0 //-> switch ASM value below, if all files in codeblocks project
    #define DISASM 1 //-> use maker.bat


    #if NON_DISASM
        #define ASM 0    // 1 to compile asma.cpp, 0 to compile proc.cpp
        #define PROC

        #if ASM
            #undef PROC
        #else
            #undef ASM
        #endif

    #endif

#endif


const int MY_SIGN = 'MM';


const int CMD_MASK = 0b00011111; // Is used to extract first 5 bytes of command code


const int RAM_MASK = 0b10000000, REG_MASK = 0b01000000, IMM_MASK = 0b00100000,
          REG_REL_MASK = 0b11000000; // <- register relative addressing mask


const int MAX_ARGS_NUM = 1, MAX_REG_NUM = 4, NON_VIDEO_RAM_SIZE = 10;


const size_t VIDEO_WEEDTH = 7, VIDEO_HEIGHT = 7;


const int RAM_SIZE = NON_VIDEO_RAM_SIZE + VIDEO_WEEDTH * VIDEO_HEIGHT;


struct bin_info
    {
    size_t signature;
    size_t bin_size;
    size_t version;
    };


struct proc
    {
    stack stk;
    size_t bin_size;
    int* recipe;
    int ip;
    FILE* prc_log;
    int reg[MAX_REG_NUM];
    int ram[RAM_SIZE];
    };


#define DEF_CMD(name, id, args_num, code) cmd_##name = id,
enum cmd_enum
    {
    unknow_cmd = -1,
    #include "cmd.h"
    version,
    };
#undef DEF_CMD


enum ARGS_NUM
    {
    ZERO_ARGS = 0,
    ONE_ARG = 1,
    TWO_ARGS = 2,
    };


// ASMA' FUNCTIONS

/*!
@brief Reads code into line_buf object
*/
line_buf* get_code (char* file_name, size_t* lines_num);


/*!
@brief Creates binary from code text file
*/
int* create_binary (line_buf* code, size_t lines_num, size_t* bin_size);


/*!
@brief Write integer array to file
*/
void write_binary (int* binary, size_t bin_size);


// DISASM' FUNCTIONS


/*!
@brief Read binary and disassemble it to disasm.txt
*/
void recogniser (int bin_size, int* cooking_list, FILE* disasm);


// PROC' COMMANDS *?


void push (proc* prc, int val);


int pop (proc* prc);


void add (proc* prc);


void out (proc* prc);


void hlt (proc* prc);


void jump (proc* prc, size_t jump_to);


void in (proc* prc);


void vm_show (proc* prc);


void circle (proc* prc);


void make_dot (proc* prc, int x, int y);


// PROC' SERVICE FUNCTIONS


/*!
@brief Establishes values for correct code execution
*/
void proc_ctor (proc* prc, size_t bin_size, int* cooking_list, FILE* log, FILE* prc_log);


/*!
@brief Dump proc to "proc_log.txt"
*/
void proc_dmp (proc* prc);


/*!
@brief Execute code from "code.txt"
*/
void doer (proc* prc);


/*!
@brief Execute exact command. Is called from doer ()
*/
void do_cmd (proc* prc, cmd_enum cmd);


#endif // PROCESSOR_H_INCLUDED
