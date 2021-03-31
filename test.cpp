#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char const *argv[])
{
    std::ifstream file("test.txt");

    std::cout << file.tellg();

    char data;
    while (file.get(data)) {
        std::cout << file.tellg();
    }

    char wait;
    std::cin >> wait;

    return 0;
}
