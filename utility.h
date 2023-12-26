#pragma once

#include <regex>
#include <string>

using std::string;

class error {
public:
    error() = default;
    error(const char* msg) : msg(string(msg)) {}

    string message() const {
        return msg;
    }

private:
    string msg;
};

enum class CXX_STANDARD : uint8_t {
    CXX_11 = 11,
    CXX_14 = 14,
    CXX_17 = 17,
    CXX_20 = 20,
    CXX_23 = 23,
};

inline string find_replace(const string& in_str, const string& match, const string& repl_str) {
    return std::regex_replace(in_str, std::regex(match), repl_str);
}