#include "printf_utils.h"
#include <iostream> // For std::cout, std::endl
#include <string>   // For std::string
#include <cstdio>   // For printf

namespace UniversalPrint {

    // NOTE: The generic print_arg<T> is defined in printf_utils.h
    // Specializations follow:

    // Specialization for int
    template<>
    void print_arg<int>(const int& arg) {
        printf("%d", arg);
    }

    // Specialization for double
    template<>
    void print_arg<double>(const double& arg) {
        printf("%f", arg);
    }

    // Specialization for const char* (C-style strings)
    template<>
    void print_arg<const char*>(const char* const& arg) {
        printf("%s", arg);
    }

    // Specialization for std::string
    template<>
    void print_arg<std::string>(const std::string& arg) {
        printf("%s", arg.c_str());
    }
    
    // Add a specialization for char if needed, otherwise it might print as a number
    template<>
    void print_arg<char>(const char& arg) {
        printf("%c", arg);
    }

} // namespace UniversalPrint

// The universal_printf function itself is defined in the header (template).
// If it were not fully defined in the header (e.g. if it was not a template itself, 
// or if we opted for a different design), its definition would go here.
