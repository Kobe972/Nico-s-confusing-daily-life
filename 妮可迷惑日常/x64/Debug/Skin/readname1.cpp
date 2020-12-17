#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
int main() {
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    std::string tmp;
    while (std::getline(std::cin, tmp)) {
        std::stringstream ss(tmp);
        for (int i = 0;i < 4; ++i) ss >> tmp;
        std::cout << tmp << std::endl;
    }
    return 0;
}
