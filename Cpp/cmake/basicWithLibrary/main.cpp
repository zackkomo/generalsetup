#include <iostream>

#include "BWLConfig.h"
#ifdef USE_LIBNAME
#include "libraryName.h"
#endif

int main(int argc, char* argv[]) {
    std::cout << argv[0] << " Version " << BWL_VERSION_MAJOR << "."
              << BWL_VERSION_MINOR << std::endl;

#ifdef USE_LIBNAME
    printSomething();
#else
    std::cout << "Not Using Library" << std::endl;
#endif

    return 0;
}
