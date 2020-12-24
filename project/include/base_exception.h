//
// Created by saptded on 15.12.2020.
//

#ifndef STREAM_PLATFORM__BASE_EXCEPTION_H_
#define STREAM_PLATFORM__BASE_EXCEPTION_H_

#include <exception>
#include <string>

class BaseException : public std::exception {
 public:
    explicit BaseException(std::string error);
    [[nodiscard]] const char* what() const noexcept override;

 private:
    std::string _error;
};

#endif  // STREAM_PLATFORM__BASE_EXCEPTION_H_
