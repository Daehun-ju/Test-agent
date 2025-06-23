#ifndef IOX_STRING_DATA_HPP
#define IOX_STRING_DATA_HPP

#include "iceoryx_hoofs/cxx/string.hpp" // For iox::cxx::string

// Define a struct to hold the string message.
// Using a struct is good practice for iceoryx data types.
// We'll use a string with a capacity of 100 characters.
// This should be sufficient for "hello" and allows for future flexibility.
struct StringData
{
    StringData() = default; // Default constructor

    StringData(const char* init_message) : message(init_message)
    {
    }

    iox::cxx::string<100> message;
};

#endif // IOX_STRING_DATA_HPP
