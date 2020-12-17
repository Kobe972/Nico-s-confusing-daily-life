#include <iostream>
#include <cstdlib>
#include <string>
int main() {
    int to;
    scanf("%d", &to);
    freopen("out.txt", "r", stdin);
    std::string cmd, name;
    while (std::getline(std::cin, name)) {
        cmd = "rename " + name + " ";
        name[4] = to + '0';
        cmd += name;
        system(cmd.c_str());
    }
    return 0;
}

