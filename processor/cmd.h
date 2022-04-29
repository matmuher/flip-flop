DEF_CMD(hlt,       0, ZERO_ARGS, HLT)

DEF_CMD(push,      1, ONE_ARG,   PUSH(ARG1))

DEF_CMD(pop,       2, ONE_ARG,   POPR(ARG1))

DEF_CMD(add,       3, ZERO_ARGS, PUSH(POP + POP))

DEF_CMD(sub,       4, ZERO_ARGS, PUSH(POP - POP))

DEF_CMD(mlt,       5, ZERO_ARGS, PUSH(POP * POP))

DEF_CMD(saw,       6, ZERO_ARGS, PUSH(POP / POP))

DEF_CMD(out,       7, ZERO_ARGS, OUT)

DEF_CMD(jump,      8, ONE_ARG,   JUMP(ARG1))

DEF_CMD(ja,        9, ONE_ARG,   CONDITION_JUMP(POP > POP))

DEF_CMD(jb,       10, ONE_ARG,   CONDITION_JUMP(POP < POP))

DEF_CMD(jae,      11, ONE_ARG,   CONDITION_JUMP(POP >= POP))

DEF_CMD(in,       12, ZERO_ARGS, IN)

DEF_CMD(say,      13, ZERO_ARGS, say ("[Meow]"))

DEF_CMD(jbe,      14, ONE_ARG,   CONDITION_JUMP(POP <= POP))

DEF_CMD(je,       15, ONE_ARG,   CONDITION_JUMP(POP == POP))

DEF_CMD(sayn,     16, ZERO_ARGS, say ("No roots"))

DEF_CMD(sqrt,     17, ZERO_ARGS, PUSH(SQRT(POP)))

DEF_CMD(sayi,     18, ZERO_ARGS, say ("Infinity roots"))

DEF_CMD(vsh,      19, ZERO_ARGS, VM_SHOW)

DEF_CMD(circ,     20, ZERO_ARGS, CIRC)

DEF_CMD(make_dot, 21, ZERO_ARGS, MAKE_DOT)

DEF_CMD(r_cos,    22, ZERO_ARGS, PUSH(R_COS))

DEF_CMD(r_sin,    23, ZERO_ARGS, PUSH(R_SIN))

DEF_CMD(ret,      24, ZERO_ARGS, JUMP(POP))

DEF_CMD(call,     25, ONE_ARG,   PUSH(prc->ip + 1); JUMP(ARG1))

DEF_CMD(jne,      26, ONE_ARG,   CONDITION_JUMP(POP != POP))
