
// reg[1] = bx

#define PUSH(ARG)                       \
    {                                   \
    int arg = ARG;                      \
    if ((cmd & CMD_MASK) == cmd_push)   \
        {                               \
        if ((cmd & REG_REL_MASK) == REG_REL_MASK) \
            {                                     \
            push (prc, prc->ram[prc->reg[1] + arg]); \
            }                           \
        else if (cmd & IMM_MASK)        \
            {                           \
            push (prc, arg);            \
            }                           \
        else if (cmd & REG_MASK)        \
            {                           \
            push (prc, prc->reg[arg]);  \
            }                           \
        else if (cmd & RAM_MASK)        \
            {                           \
            push (prc, prc->ram[arg]);  \
            }                           \
        }                               \
    else                                \
        {                               \
        push (prc, arg);                \
        }                               \
    }


#define POPR(ARG)                                \
    {                                            \
    int arg = ARG;                               \
                                                 \
    if ((cmd & REG_REL_MASK) == REG_REL_MASK)    \
        {                                        \
        prc->ram[prc->reg[1] + arg] = POP;       \
        }                                        \
    else if (cmd & REG_MASK)                     \
        {                                        \
        prc->reg[arg] = POP;                     \
        }                                        \
    else if (cmd & RAM_MASK)                     \
        {                                        \
        prc->ram[arg] = POP;                     \
        }                                        \
    else if (cmd & IMM_MASK)                     \
        {                                        \
        POP;                                     \
        }                                        \
    }                                            \



#define POP pop (prc)


#define JUMP(ARG) jump (prc, ARG)


#define CONDITION_JUMP(CONDITION)\
    if (CONDITION)               \
        JUMP(ARG1)


#define OUT out (prc)


#define IN in (prc)


#define ARG1 prc->recipe[prc->ip]


#define HLT hlt (prc)


// Video memory show
#define VM_SHOW vm_show (prc)


#define CIRC circle (prc)


#define MAKE_DOT {make_dot (prc, POP, POP);} // x, y


// Transfer from polar to cartesian coordinates
#define R_COS (int) round (cos (POP * M_PI / 180) * POP)


#define R_SIN (int) round (sin (POP * M_PI / 180) * POP)


#define SQRT(ARG) (int) round (sqrt (ARG))
