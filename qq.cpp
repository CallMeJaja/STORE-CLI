#include "conio.h"
#include "iostream"

int main() {
    int ip;

    while (true) {
        std::cout << ">> : ", std::cin >> ip;

        if (kbhit()) {
            std::cout << getch();
        }
    }

    return 0;
}