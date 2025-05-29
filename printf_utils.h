#ifndef PRINTF_UTILS_H
#define PRINTF_UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <variant> // Or <any>
#include <cstdio>  // For printf

// Forward declaration of the helper function/struct if needed for implementation
namespace UniversalPrint {
    // Helper to print individual arguments
    // Definition of the generic template must be in the header for instantiation
    template<typename T>
    void print_arg(const T& arg) {
        // Attempt to print using std::cout. This will be the fallback.
        std::cout << arg;
    }

    // Forward declare specializations that will be defined in the .cpp
    // This tells the compiler that these specific types have custom implementations elsewhere.
    template<> void print_arg<int>(const int& arg);
    template<> void print_arg<double>(const double& arg);
    template<> void print_arg<const char*>(const char* const& arg);
    template<> void print_arg<std::string>(const std::string& arg);
    template<> void print_arg<char>(const char& arg);
}

// Variadic template function declaration
template<typename... Args>
void universal_printf(const Args&... args) {
    // Implementation will expand the arguments and call print_arg for each
    // This can be done using a fold expression (C++17) or recursion.
    // For C++17 and later:
    (UniversalPrint::print_arg(args), ...);
    std::cout << std::endl; // Add a newline at the end, like println
}

#endif // PRINTF_UTILS_H
