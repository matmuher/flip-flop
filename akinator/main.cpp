#include <stdio.h>
#include <stdlib.h>
#include <string.h>



enum VERBOSE_LVLS
    {
    ZERO_LVL = 0,
    FIRST_LVL = 1,
    SECOND_LVL = 2,
    };


#define VERBOSE SECOND_LVL


struct node
    {
    const char* content; //<- C-type content string

    node* left_child;
    node* right_child;

    unsigned int node_type : 1; //<- SIGN or OBJECT
    };


enum node_type
    {
    SIGN = 0,
    OBJECT = 1,
    };


struct tree
    {
    node* root;
    };


node* create_node (const char* content_to_push)
    {
    char* node_content = (char*) calloc (1, (strlen (content_to_push) + 1) * sizeof (char));
    strcpy (node_content, content_to_push);


    node* new_node = (node*) calloc (1, sizeof (node));
    new_node->content = node_content;


    new_node->left_child = NULL;
    new_node->right_child = NULL;


    return new_node;
    }


void print_node (node* node_to_print)
    {
    // printf ("node address: %p\n", node_to_print);
    if (node_to_print != NULL)
        {
        printf (" %s \n", node_to_print->content);
        }
    else
        {
        printf (" empty \n");
        }
    }


FILE* graph_dump_create ()
    {
    FILE* graph_dump = fopen ("ak.dot", "w");

    fputs ("digraph G {\n", graph_dump);


    return graph_dump;
    }


void graph_dump_show ()
    {
    system ("dot ak.dot -Tpng -o ak.png");
    system ("start ak.png");
    }


void graph_dump_close (FILE* graph_dump)
    {
    fputs ("}\n", graph_dump);

    fclose (graph_dump);

    // Add show off regime
    graph_dump_show ();
    }


void dot_connect_nuclear_family (node* parent_node, FILE* graph_log)
    {
    if (parent_node != NULL)
        {
        fprintf (graph_log, "A%p [label = \"%s\"];\n", parent_node, parent_node->content);

        if (parent_node->left_child != NULL)
            {
            fprintf (graph_log, "A%p -> A%p;\n", parent_node, parent_node->left_child);
            }

        if (parent_node->right_child != NULL)
            {
            fprintf (graph_log, "A%p -> A%p;\n", parent_node, parent_node->right_child);
            }
        }
    }


void graphviz_dump (node* current_node, FILE* graph_log)
    {
    if (current_node != NULL)
        {
        dot_connect_nuclear_family (current_node, graph_log);

        graphviz_dump (current_node->left_child, graph_log);

        graphviz_dump (current_node->right_child, graph_log);
        }
    }


// Mode generalize: node_visitor gets function pointer
void node_printor (node* current_node)
    {
    if (current_node != NULL)
        {
        print_node (current_node);

        node_printor (current_node->left_child);

        node_printor (current_node->right_child);
        }
    else
        {
        print_node (current_node);
        }
    }


enum LOGIC
    {
    INCORRECT = -1,
    NO = 0,
    YES = 1,
    };


void clear_buffer ()
    {
    scanf ("%*s");

    fprintf (stdin, "kiki\n");
    fflush (stdout);
    }


int get_answer ()
    {
    const size_t MAX_ANSWER_LENGTH = 25;

    char user_answer[MAX_ANSWER_LENGTH] = {};
    scanf (" %s", user_answer);
    clear_buffer ();

    const int ATTEMPTS_ALLOWED = 5;

    for (int attempt_id = 0; attempt_id < ATTEMPTS_ALLOWED; attempt_id++)
        {
        if (strcmp (user_answer, "No") == 0)
            {
            return NO;
            }
        else if (strcmp (user_answer, "Yes") == 0)
            {
            return YES;
            }
        else
            {
            puts ("Incorrect vvod, kojanii meshok. Please, use Y/N and try again");
            scanf (" %s ", user_answer);
            clear_buffer ();
            }
        }

    puts ("Sorry, you're ne obuchaem\n");
    exit (777);
    }

const int MAX_SIGN_LENGTH = 20;


void add_object (node* razvilka_node, const char* basa_object, const char* user_object)
    {
    printf ("Skajite-ka, chem %s distinguish'aetsya ot %s?\n", user_object, basa_object);

    char user_sign[MAX_SIGN_LENGTH];
    scanf (" %s ", &user_sign);
    clear_buffer ();

    char* razvilka_sign = (char*) calloc (1, (strlen (user_sign) + 1) * sizeof (char));
    strcpy (razvilka_sign, user_sign);


    razvilka_node->content = razvilka_sign;
    razvilka_node->node_type = SIGN;

    razvilka_node->right_child = create_node (user_object);
    razvilka_node->node_type = OBJECT;

    razvilka_node->left_child = create_node (basa_object);
    razvilka_node->node_type = OBJECT;
    }


const int MAX_OBJECT_NAME_LENGTH = 20;


void ask_object (node* node_object)
    {
    if (node_object != NULL)
        {
        const char* basa_object = node_object->content;


        printf ("Vi zagadali %s?\n", basa_object);


        int answer = INCORRECT;
        answer =  get_answer ();


        if (answer == YES)
            {
            printf ("Ama very clavar mashine!!! Your boy next door is %s\n", basa_object);
            }
        else if (answer == NO)
            {
            puts ("Togda kolis' shto zagadal!:");
            char new_object_name[MAX_OBJECT_NAME_LENGTH] = {};
            scanf ("%s", new_object_name);
            clear_buffer ();

            add_object (node_object, basa_object, new_object_name);

            printf ("%s kotorii %s was successfully added to data base! Thx)\n", new_object_name, node_object->content);
            }
        else
            {
            puts ("[ERROR] get_answer worked incorrectly");
            }
        }
    #if VERBOSE > FISRT_LVL
    else
        {
        puts ("Object is empty");
        }
    #endif
    }


node* ask_sign (node* sign_node)
    {
    const char* sign = sign_node->content;
    printf ("Ono %s?\n", sign);


    int answer = INCORRECT;
    answer = get_answer ();


    if (answer == YES)
        {
        return sign_node->right_child;
        }
    else if (answer == NO)
        {
        return sign_node->left_child;
        }
    else
        {
        puts ("[ERROR] get_answer worked incorrectly");
        }
    }

const int GAME_IS_ON = 1;


node* start_akinator ()
    {
    node* root = create_node ("Nevedomaya dich");
    root->node_type = OBJECT;

    while (GAME_IS_ON)
        {
        puts ("Hello, I'm akinator. Please, guess somebody\n"
              "Now i will try to find out who is it");

        node* current_node = root;

        while (current_node->node_type == SIGN)
            {
            current_node = ask_sign (current_node);
            }

        ask_object (current_node);

        puts ("Do u wanna play again?");

        int answer = INCORRECT;
        answer = get_answer ();

        if (answer == NO)
            {
            break;
            }
        }


    return root;
    }


int main ()
    {
    node* root = start_akinator ();


    FILE* graph_dump = graph_dump_create ();

    graphviz_dump (root, graph_dump);

    graph_dump_close (graph_dump);
    }
