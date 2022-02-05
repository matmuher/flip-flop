#include <stdio.h>
#include <string.h>
#include "great_grammar.h"
#include "lexo_parser.h"
#include "..\hamlet\d_hamlet_functions.h"

void to_asm (node* root);

#if 0
node* left_grow (const char* binder_node_content,
                 node* previous_node, node* new_val,
                 char delimiter,
                 int (*exit_condition)(token* current_token),
                 parsed_line_reader* pl_reader)
    {
    node* val = get_A (pl_reader);
    require (delimiter, pl_reader);

    st_begunok->right_child = val;

    if (gfs(pl[gfs(token_id)].type) == T_END)
        {
        break;
        }

    node* temp = st (NULL, NULL);

    temp->left_child = st_begunok;

    st_begunok = temp;
    }
#endif

int main ()
    {
    #if 1 // Sueta
        char* file_name = "wasup_world.cum";
        size_t lines_num = 0;
        line_buf* lines =  get_strings (file_name, &lines_num, true);

        token* parsed_line = lexo_parse (lines[0].beg_ptr);

        parsed_line_reader pl_reader = {0, parsed_line};

        #if 1 // Check lexo parse

            print_pl (&pl_reader);

        #endif

        #if 1 // Recursive parse

            node* root = get_G (&pl_reader);

            puts ("Descent parsing ended");

            dot_this_shit (root);

            // to_asm (root);
        #endif

        memory_free ();

    #endif
    }
