#include "ExceptionHandling.h"


const char* NNG_Exception::what() const noexcept {
    return message.c_str();
}