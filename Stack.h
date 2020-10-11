#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>



#define ASSERT_OK(pointer) if (Stack_OK(pointer) != OK)  \
                           {                             \
                                Stack_Dump(pointer);     \
                                assert(!"OK");           \
                           }                             \



typedef size_t error_t;


enum errors
{
    OK                  = 0,

    STACK_EMPTY         = 1,

    POINTER_STRUCT_NULL = 2,

    POINTER_DATA_NULL   = 3,

    CAPACITY_LESS_SIZE  = 4,

    INCORRECT_MODE      = 5,

    INCORRECT_PUSH      = 6,

    NOT_DESTRUCTED      = 7,

    CAPACITY_EMPTY      = 8,

    ELEM_IS_NAN         = 9,

    ERROR_LOG_FILE      = 10,

    CANARY_BEGIN_DEAD   = 11,

    CANARY_END_DEAD     = 12,

    CANARY_DATA_1_DEAD  = 13,

    CANARY_DATA_2_DEAD  = 14,

    HASH_INCORRECT      = 15
};

const size_t INCREASE_FACTOR = 2;
const size_t DECREASE_FACTOR = 4;
const size_t RESIZE_FACTOR   = 2;
const size_t MAX_SIZE_T      = -1;
const size_t INCREASE        = 1;
const size_t DECREASE        = 2;

const unsigned long long CANARY_1 = 0xC0FFEEABC;
const unsigned long long CANARY_2 = 0xABCC0FFEE;

struct Stack_t
{
    long long int      hash_value_;
    unsigned long long canary_begin_;
    size_t  size_;
    size_t  capacity_;
    double* data_;
    unsigned long long canary_end_;
};


Stack_t* StackConstruct  (size_t capacity);

error_t  StackDestruct   (Stack_t* pointer);

error_t  StackResize     (Stack_t* pointer, size_t resize_factor, size_t mode);

error_t  StackPush       (Stack_t* pointer, double elem);

double   StackPop        (Stack_t* pointer);

error_t  Stack_OK        (Stack_t* pointer);

error_t  Stack_Dump      (Stack_t* pointer);

void     PoisonSpace     (double* begin, double* end);

double*  DataConstruct   (size_t capacity);


long long int HashCounter(void* pointer_begin, void* pointer_end);
void          HashUpdate (Stack_t* pointer);
error_t       HashCheck  (Stack_t* pointer);


void     Stack_Unit_Tests (void);
void     StackPush_Test   (void);
void     StackPop_Test    (void);
void     StackResize_Test (void);
void     LogFile_Test     (void);

