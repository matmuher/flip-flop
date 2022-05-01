#ifndef ELEPHANT_FUNCTIONS_H_INCLUDED
#define ELEPHANT_FUNCTIONS_H_INCLUDED

const size_t MAX_PTRS_NUM = 1000;

/*!
@brief Structure for containing pointers to
       dynamically allocated memory
*/
struct death_note
    {
    void* ptrs_list[MAX_PTRS_NUM];
    int grosse;
    } extern std_note;


/*!
@brief Calls calloc function from stdlib.h and
       memorizes pointer to allocated memory
@details First 2 parameters match calloc'c ones\n
         3d parameter is not obligate. It determines in what structure\n
         pointers will be written. If you want you can create your\n
         variable of death_note structure.\ò
         If so you must write 4th argument during every call of
         function from elephant_functions.h\n
*/
void* elephant_calloc (size_t elements_num, size_t element_size, death_note* note = &std_note);

// [!!!] if you put there pointer from elephant_calloc bad things will occur
// because death_not has already remembered elephant_realloc's pointer
// and memory_free will try to free () it
// but after realloc () implementing it will not be dynamic allocated memory anymore
// To conclude: you can use elephant_realloc ()
// but only with standard calloc'ed or malloc'ed pointers
/*!
@brief Calls realloc function from stdlib.h and
       memorizes pointer to reallocated memory
@details 1st parameter matches realloc's first one
         2nd and 3d parameters match calloc'c parameters\n
         4th parameter is not obligate. It determines in what structure\n
         pointers will be written. If you want you can create your\n
         variable of death_note structure.\ò
         If so you must write 4th argument during every call of
         function from elephant_functions.h\n
*/
void* elephant_realloc (void* user_ptr, size_t elements_num,
                        size_t element_size, death_note* note = &std_note);

/*!
@brief Clear dynamically allocated memory
@details Iterates elements of void* array from death_note* note
         and apply free () function to the content of each
*/
void memory_free (death_note* note = &std_note);

#endif
