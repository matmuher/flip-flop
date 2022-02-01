#include <stdlib.h>


struct OWList
    {
    int lst_size;


    int* data;


    int* next;


    int tail, head;
    };


struct List
    {
    int lst_size;


    double* data;


    double* sorted_data;


    int* next;


    int tail, head;


    int* prev;


    FILE* graph_log;


    FILE* text_log;


    OWList free;


    unsigned int linear_status : 1;
    };


const int FREE_CELL = -1,
          EMPTY_LST_EDGE = 0;


enum ERRORS
    {
    LST_IS_EMPTY = -1,


    LST_IS_FULL = 0,
    };


// USER'S FUNCTIONS

/*!
@brief Creates double_linked list
@details Initialization, memory  allocation
@params[in] lst pointer to unprepared List object
@params[in] lst_size list start size
@params[out] lst pointer to prepared List object
*/
void lst_ctor (List* lst, size_t lst_size);


/*!
@brief Deletes double_linked list
@details Frees memory
*/
void lst_dtor (List* lst);


/*!
@brief Puts element after the last one
*/
int lst_insert_back (List* lst, double value);


/*!
@brief Puts element after the specified one
@params [in] lst Double_linked list pointer
@params [in] value value to put
@params[in] insert_after_this_cell_id physical index of specified element
@return physical address of put value
*/
int lst_insert_after (List* lst, double value, size_t insert_after_this_cell_id);


/*!
@brief Delete the specified element
@params [in] lst Double_linked list pointer
@params[in] delete_cell_id physical index of specified element
*/
void lst_delete_cell (List* lst, size_t delete_cell_id);


/*!
@brief Silly console dump
*/
void lst_dmp (List* lst);


/*!
@brief Graphic dump
*/
void cool_dmp (List* lst);


/*!
@brief Shows graphic dump
*/
void cool_dmp_show ();


/*!
@brief Silly linearizing
*/
void lst_linearize (List* lst);


/*!
@brief Graph dump using html tags
*/
void graph_dmp (List* lst);
