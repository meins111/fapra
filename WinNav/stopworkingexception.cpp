#include "stopworkingexception.h"

StopWorkingException::StopWorkingException() : message("Update stopped running")
{

}

const char *StopWorkingException::what() const noexcept {
    return message.c_str();
}

