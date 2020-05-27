#include "Exception.hpp"


Exception::Exception(std::string error) : error_message(error) {}

Exception::~Exception() {}

const char* Exception::what() const noexcept { return error_message.c_str(); }
