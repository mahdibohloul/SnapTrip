#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <exception>
#include <string>

class Exception : public std::exception
{
public:
    Exception(std::string error);
    ~Exception();
    const char* what() const noexcept;

private:
    std::string error_message;
};

#endif
