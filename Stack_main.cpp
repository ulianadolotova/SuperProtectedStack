#include "Stack.h"



int main (int argc, const char* argv[])
{
    if (argc > 1 && strcmp(argv[1], "--test") == 0)
    {
          Stack_Unit_Tests ();
    }

}
