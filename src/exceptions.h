#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <iostream>

class Error : public std::exception {
private:
    std::string message;
public:
    Error(const std::string& msg) : message(msg) {};
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class ConfigError : public Error {
public:
    explicit ConfigError(const std::string& msg) : Error(msg) {}
};

class ReaderError : public Error {
public:
    explicit ReaderError(const std::string& msg) : Error(msg) {}
};


#endif //EXCEPTIONS_H
