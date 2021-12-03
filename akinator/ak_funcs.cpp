#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ak.h"


#define VERBOSE SECOND_LVL





void add_object (node* razvilka_node, const char* basa_object, const char* user_object)
    {
    printf ("Skajite-ka, chem %s distinguish'aetsya ot %s?\n", user_object, basa_object);

    char user_sign[MAX_SIGN_NAME_LENGTH];
    safe_gets (user_sign, MAX_SIGN_NAME_LENGTH);

    char* razvilka_sign = (char*) calloc (1, (strlen (user_sign) + 1) * sizeof (char));
    strcpy (razvilka_sign, user_sign);


    razvilka_node->content = razvilka_sign;
    razvilka_node->node_type = SIGN;

    razvilka_node->right_child = create_node (user_object);
    razvilka_node->right_child->node_type = OBJECT;

    razvilka_node->left_child = create_node (basa_object);
    razvilka_node->left_child->node_type = OBJECT;
    }


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
            safe_gets (new_object_name, MAX_OBJECT_NAME_LENGTH);

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


node* start_akinator (void)
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
