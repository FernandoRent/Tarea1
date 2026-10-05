#pragma once

#include <iostream>
#include <string>

struct Cuadruplo {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string res;
};

inline std::ostream& operator<<(std::ostream& os, const Cuadruplo& q) {
    os << "(" << q.op << ", " << q.arg1 << ", " << q.arg2 << ", " << q.res << ")";
    return os;
}