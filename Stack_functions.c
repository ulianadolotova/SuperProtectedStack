#include "Stack.h"



//------------------------------------------------------------------------------
//! This function creates stack.
//!
//! @param [in] capacity Spaciousness of stack
//!
//! @return Function returns pointer to the stack.
//------------------------------------------------------------------------------
Stack_t* StackConstruct (size_t capacity)
{
    Stack_t* pointer   = (Stack_t*) calloc (capacity, sizeof (Stack_t));

    assert (pointer);

    pointer->hash_value_   = 0;
    pointer->canary_begin_ = CANARY_1;
    pointer->data_         = DataConstruct (capacity);
    pointer->size_         = 0;
    pointer->capacity_     = capacity;
    pointer->canary_end_   = CANARY_2;

    PoisonSpace (pointer->data_, pointer->data_ + pointer->capacity_);

    HashUpdate (pointer);
    ASSERT_OK(pointer)

    return pointer;
}



//---------------------------------------------------------------------------------
//! This function destructs stack.
//!
//! @param [out] pointer Pointer to the stack
//!
//! @return Function returns OK, if there is no mistake,
//!                  returns NOT_DESTRUCTED, if there is mistake.
//----------------------------------------------------------------------------------
error_t StackDestruct (Stack_t* pointer)
{
    PoisonSpace (pointer->data_, pointer->data_ + pointer->capacity_);

    free ((char*) pointer->data_ - sizeof (unsigned long long));

    pointer->canary_begin_ = 0;
    pointer->capacity_     = 0;
    pointer->size_         = 0;
    pointer->data_         = NULL;
    pointer->canary_end_   = 0;

    free (pointer);

    pointer = NULL;

    return (pointer == NULL) ? OK : NOT_DESTRUCTED;
}



//-------------------------------------------------------------------------------------
//! This function changes stack's capacity.
//!
//! @param [out] pointer       Pointer to the stack
//! @param [in]  resize_factor Coefficient of change
//! @param [out] mode          Mode of change (increase or decrease)
//!
//! @return Function returns OK, if there is no mistake,
//!                  returns INCORRECT_MODE, if the mode is incorrect.
//-------------------------------------------------------------------------------------
error_t StackResize (Stack_t* pointer, size_t resize_factor, size_t mode)
{
    HashUpdate (pointer);
    ASSERT_OK(pointer)

    if (mode == INCREASE)
    {
        size_t capacity_old = pointer->capacity_;
        pointer->capacity_ *= resize_factor;
        size_t new_size     = pointer->capacity_ * sizeof(double) + 2 * sizeof(unsigned long long);

        pointer->data_ = (double*) ((char*) pointer->data_ - sizeof (unsigned long long));
        pointer->data_ = (double*) ((char*) realloc ((char*) pointer->data_, new_size));

        *((unsigned long long*) pointer->data_) = CANARY_1;

        pointer->data_ = (double*) ((char*) pointer->data_ + sizeof (unsigned long long));

         *((unsigned long long*) (pointer->data_ + pointer->capacity_)) = CANARY_2;

        size_t capacity_new = pointer->capacity_;
        PoisonSpace (pointer->data_ + capacity_old, pointer->data_ + capacity_new);

        HashUpdate (pointer);
        ASSERT_OK(pointer);

        return OK;
    }

    else if (mode == DECREASE)
    {
        pointer->capacity_ /= resize_factor;
        pointer->capacity_++;
        size_t new_size     = pointer->capacity_ * sizeof(double) + 2 * sizeof(unsigned long long);

        pointer->data_ = (double*) ((char*) pointer->data_ - sizeof (unsigned long long));
        pointer->data_ = (double*) ((char*) realloc ((char*) pointer->data_, new_size));

        *((unsigned long long*) pointer->data_) = CANARY_1;

        pointer->data_ = (double*) ((char*) pointer->data_ + sizeof (unsigned long long));

         *((unsigned long long*) (pointer->data_ + pointer->capacity_)) = CANARY_2;

        HashUpdate (pointer);
        ASSERT_OK(pointer)

        return OK;
    }

    else
    {
        HashUpdate (pointer);
        ASSERT_OK(pointer)

        return INCORRECT_MODE;
    }
}



//-------------------------------------------------------------------------------------
//! Functions pushes element to the stack.
//!
//! @param [out] pointer Pointer to the stack
//! @param [in]  elem    Element to be pushed
//!
//! @return Functions returns OK, if there is no mistake,
//!                   returns INCORRECT_PUSH, if the element was pushed incorrectly
//--------------------------------------------------------------------------------------
error_t StackPush (Stack_t* pointer, double elem)
{
    HashUpdate (pointer);
    ASSERT_OK(pointer)

    if (pointer->size_ == pointer->capacity_)
    {
        StackResize (pointer, RESIZE_FACTOR, INCREASE);
    }

    assert (pointer->size_ < pointer->capacity_);
    pointer->data_[pointer->size_++] = elem;

    HashUpdate (pointer);
    ASSERT_OK(pointer)

    return (pointer->data_[pointer->size_ - 1] == elem) ? OK : INCORRECT_PUSH;

}



//-------------------------------------------------------------------------------------
//! Functions takes last element out of stack.
//!
//! @param [out] pointer Pointer to the stack
//!
//! @return Functions returns element, which was taken out of stack.
//--------------------------------------------------------------------------------------
double StackPop (Stack_t* pointer)
{
    HashUpdate (pointer);
    ASSERT_OK(pointer)

    double elem = pointer->data_[--pointer->size_];

    pointer->data_[pointer->size_] = NAN;

    if (DECREASE_FACTOR * pointer->size_ < pointer->capacity_)
    {
        StackResize (pointer, RESIZE_FACTOR, DECREASE);
    }

    HashUpdate (pointer);
    ASSERT_OK(pointer)

    return elem;
}



//-----------------------------------------------------------------------------------
//! This functions puts NaN in the elements of array at half-interval [begin, end)
//!
//! @param [out] begin Pointer to the begin
//! @param [out] end   Pointer to the end
//------------------------------------------------------------------------------------
void PoisonSpace (double* begin, double* end)
{
    size_t lenth = end - begin;

    for (size_t i = 0; i < lenth; i++)
            *(begin + i) = NAN;
}



//---------------------------------------------------------------------------------------------------
//! This function creates array for values with 2 canaries
//!
//! @param [in] capacity Number of values
//!
//! @return Function returns pointer to the place in the array,
//!         where the values are supposed to be
//----------------------------------------------------------------------------------------------------
double* DataConstruct (size_t capacity)
{
    char* true_pointer  =
        (char*) calloc (sizeof (double) * capacity + sizeof (unsigned long long) * 2, sizeof (char));

    *((unsigned long long*) true_pointer) = CANARY_1;

    double* fixed_pointer = (double*) (true_pointer + sizeof (unsigned long long));

    *((unsigned long long*) (fixed_pointer + capacity)) = CANARY_2;

    return fixed_pointer;
}
