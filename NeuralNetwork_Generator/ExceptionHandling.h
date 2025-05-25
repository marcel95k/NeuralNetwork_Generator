#pragma once
#include <iostream>
#include <exception>
#include <string>

// Eigene Exception-Klasse
class NNG_Exception : public std::exception {

private:
    std::string message;
public:
    explicit NNG_Exception(const std::string& msg) : message(msg){}
    const char* what() const noexcept override;
};
