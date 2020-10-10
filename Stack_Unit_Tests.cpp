#include "Stack.h"



void Stack_Unit_Tests (void)
{
    StackPush_Test ();
    StackPop_Test ();
    StackResize_Test ();
    LogFile_Test ();
}



void StackPush_Test (void)
{
    const size_t NUM_TESTS = 20;

    printf("\nTesting StackPush function:\n");

    Stack_t* pointer = StackConstruct (NUM_TESTS);

    for (size_t i = 1; i < NUM_TESTS + 1; i++)
    {
        StackPush(pointer, (double) i);
    }

    for (size_t i = NUM_TESTS; i > 0; i--)
    {
        if (pointer->data_[pointer->size_ - 1] == i)
        {
            printf("\n# test #%d is OK\n", NUM_TESTS - i + 1);
        }

        else
        {
            printf("\n# test #%d is BAD\n", NUM_TESTS - i + 1);
        }
        pointer->size_--;
    }

    StackDestruct (pointer);

}



void StackPop_Test (void)
{
    const size_t NUM_TESTS = 20;

    printf("\nTesting StackPop function:\n");

    Stack_t* pointer = StackConstruct (NUM_TESTS);

    for (size_t i = 1; i < NUM_TESTS + 1; i++)
    {
        StackPush(pointer, (double) i);
    }

    for (size_t i = NUM_TESTS; i > 0; i--)
    {
        Stack_Dump (pointer); //
        if (StackPop (pointer) == (double) i)
        {
            printf("\n# test #%d is OK\n", NUM_TESTS - i + 1);
        }

        else
        {
            printf("\n# test #%d is BAD: exp: %d real: %lg\n", NUM_TESTS - i + 1, i, pointer->data_[pointer->size_ - 1] );
        }
    }

    StackDestruct (pointer);

}



void StackResize_Test (void)
{
    printf("\nTesting StackResize function:\n");

    const size_t NUM_TESTS = 20;

    Stack_t* pointer = StackConstruct (100);

    for (size_t i = 1; i < NUM_TESTS/2; i++)
    {

        size_t old_capacity = pointer->capacity_;

        StackResize(pointer, (double) i, INCREASE);

        if (pointer->capacity_ == old_capacity * i)
        {
            printf("\n# test #%d is OK\n", i);
        }

        else
        {
            printf("\n# test #%d is BAD\n", i);
        }
    }

    for (size_t i = NUM_TESTS/2; i < NUM_TESTS + 1; i++)
    {
        size_t old_capacity = pointer->capacity_;

        StackResize(pointer, (double) i, DECREASE);

        if (pointer->capacity_ == old_capacity / i + 1)
        {
            printf("\n# test #%d is OK\n", i);
        }

        else
        {
            printf("\n# test #%d is BAD\n", i);
        }
    }
}



void LogFile_Test (void)
{
    Stack_t* pointer = StackConstruct (100);

    for (size_t i = 0; i < 50; i++)
    {
        StackPush(pointer, (double) i);
    }

    Stack_Dump (pointer);
}



