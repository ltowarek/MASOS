#include <iostream>
#include <string>

#include "gmp.h"

int main(int argc, char** argv) {
    std::string input = "";
    while (std::cin >> input) {
        std::cout << input << std::endl;
    }
    return 0;
}
