#ifndef LOCATE_DEPS_ASSERT
#define LOCATE_DEPS_ASSERT

#include <cstdlib>
#include <iostream>

#define UNSAFE_ASSERT(x)                                                     \
    do                                                                       \
    {                                                                        \
        if(!(x))                                                             \
        {                                                                    \
            std::cerr << "assertion failed: "                                \
                      << #x                                                  \
                      << " (in " << __FILE__ << ":" << __LINE__ << ")"       \
                      << std::endl;                                          \
            std::exit(255);                                                  \
        }                                                                    \
    }                                                                        \
    while(false);

#endif