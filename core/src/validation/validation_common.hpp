#include <iostream>

#define validate_begin(name) \
    const char *section_name = name

#define validate(condition, message) \
    if (!(condition)) { \
        std::cerr << "Giygas validation: " << section_name << ": Validation failed!" << std::endl \
            << "\tAssertion: " << #condition << std::endl \
            << "\tMessage: " << message << std::endl \
            << std::endl; \
        return false; \
    }
