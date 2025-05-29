#include "printf_utils.h" // Our new library
#include <string>        // For std::string

int main() {
    universal_printf("Hello, World!");
    universal_printf("Test with int: ", 10, " and double: ", 3.14159);
    universal_printf("Char: ", 'A');

    std::string my_string = "This is a std::string.";
    universal_printf(my_string);

    universal_printf("Mixed types: ", 123, " ", "abc", " ", 45.67, 'Z');
    
    // Test the generic fallback (if std::cout is used in the generic print_arg)
    // universal_printf("Testing generic with bool: ", true); // bool might print as 1

    return 0;
}
