#ifndef STOPWORKINGEXCEPTION_H
#define STOPWORKINGEXCEPTION_H

#include <string>
#include <exception>

class StopWorkingException : std::exception
{
    std::string message;
public:
    StopWorkingException();
    const char *what() const noexcept;
};

#endif // STOPWORKINGEXCEPTION_H
