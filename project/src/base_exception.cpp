//
// Created by saptded on 15.12.2020.
//

#include "base_exception.h"

#include <cstring>

BaseException::BaseException(std::string error) : _error(std::move(error)) {
    _error += std::string(" : ") + strerror(errno);
}

const char* BaseException::what() const noexcept { return _error.c_str(); }