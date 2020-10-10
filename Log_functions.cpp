#include "Stack.h"



//--------------------------------------------------------------------------------
//! This function looks for the breakdowns in stack.
//!
//! @param [out] pointer Pointer to the stack
//!
//! @return Function returns error code if the error was found,
//!                  returns OK if the error was not found.
//---------------------------------------------------------------------------------
error_t Stack_OK (Stack_t* pointer)
{
    if (pointer->canary_begin_ != CANARY_1)
    {
        return CANARY_BEGIN_DEAD;
    }

    if (pointer->canary_end_ != CANARY_2)
    {
        return CANARY_END_DEAD;
    }

    if ( *((unsigned long long*) ((char*) pointer->data_ - sizeof (unsigned long long)) ) != CANARY_1)
    {
        return CANARY_DATA_1_DEAD;
    }

    if ( *((unsigned long long*) (pointer->data_ + pointer->capacity_)) != CANARY_2)
    {
        return CANARY_DATA_2_DEAD;
    }

    if (pointer == NULL)
    {
        return POINTER_STRUCT_NULL;
    }

    if (pointer->data_ == NULL)
    {
        return POINTER_DATA_NULL;
    }

    if (pointer->size_ > pointer->capacity_)
    {
        return CAPACITY_LESS_SIZE;
    }

    if (pointer->size_ == MAX_SIZE_T)
    {
        return STACK_EMPTY;
    }

    if (pointer->capacity_ == MAX_SIZE_T)
    {
        return CAPACITY_EMPTY;
    }

    for (size_t i = 0; i < pointer->size_; i++)
    {
        if(isnan(pointer->data_[i]))
        {
            return ELEM_IS_NAN;
        }
    }

    return OK;
}



//--------------------------------------------------------------------------------
//! This function prints to the Log_File information about stack.
//!
//! @param [out] pointer Pointer to the stack
//!
//! @return Function returns ERROR_LOG_FILE if it worked incorrectly,
//!                  returns OK if it worked correctly.
//---------------------------------------------------------------------------------
error_t Stack_Dump (Stack_t* pointer)
{
    size_t result = Stack_OK (pointer);

    char* error = NULL;

    switch (result)
    {
        case (0):
        {
            error = "OK";
            break;
        }

        case (1):
        {
            error = "STACK_EMPTY";
            break;
        }

        case (2):
        {
            error = "POINTER_STRUCT_NULL";
            break;
        }

        case (3):
        {
            error = "POINTER_DATA_NULL";
            break;
        }

        case (4):
        {
            error = "CAPACITY_LESS_SIZE";
            break;
        }

        case (8):
        {
            error = "CAPACITY_EMPTY";
            break;
        }

        case (9):
        {
            error = "ELEM_IS_NAN";
            break;
        }

        case (11):
        {
            error = "CANARY_BEGIN_DEAD";
            break;
        }

        case (12):
        {
            error = "CANARY_END_DEAD";
            break;
        }

        case (13):
        {
            error = "CANARY_DATA_1_DEAD";
            break;
        }

        case (14):
        {
            error = "CANARY_DATA_2_DEAD";
            break;
        }

        default:
        {
            error = "LOG_FILE_BUG";
        }
    }


    FILE* Log_File = fopen ("Log_File.txt", "w");

    assert (Log_File != NULL);

    fprintf (Log_File, "\nStack_t (%s) [%p]\n\n", error, pointer);

    fprintf (Log_File, "Canary in the begin of structure: %llx\n", pointer->canary_begin_);
    fprintf (Log_File, "Canary in the end of structure  : %llx\n\n", pointer->canary_end_);

    fprintf (Log_File, "Canary before data[]: %llx\n", *((unsigned long long*) ((char*) pointer->data_ - sizeof (unsigned long long)) ));
    fprintf (Log_File, "Canary after data[] : %llx\n\n",*((unsigned long long*) (pointer->data_ + pointer->capacity_)));

    fprintf (Log_File, "size     = %u\n", pointer->size_);
    fprintf (Log_File, "capacity = %u\n", pointer->capacity_);
    fprintf (Log_File, "data [%p]\n", pointer->data_);
    fprintf (Log_File, "{\n");

    for (size_t i = 0; i < pointer->capacity_; i++)
    {
        if (! isnan(pointer->data_[i]) )
            fprintf (Log_File, "\t* [%u] = %lg\n", i, pointer->data_[i]);

        else
            fprintf (Log_File, "\t  [%u] = %lg\n", i, pointer->data_[i]);
    }

    fprintf (Log_File, "}");

    int result_close = fclose (Log_File);

    return (result_close == EOF) ? ERROR_LOG_FILE : OK;
}



















