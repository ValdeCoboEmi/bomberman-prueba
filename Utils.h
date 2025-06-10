#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Utils {
    void clearScreen();
    void sleep(int milliseconds);
    void moveCursor(int x, int y);
    std::string readFile(const std::string& path);

    // Implementaciones
    void clearScreen() {
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coord = {0, 0};
        SetConsoleCursorPosition(hConsole, coord);
    #else
        std::cout << "\033[H";
    #endif
    }

    void sleep(int milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    void moveCursor(int x, int y) {
    #ifdef _WIN32
        COORD pos = {(SHORT)x, (SHORT)y};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    #else
        std::cout << "\033[" << y << ";" << x << "H";
    #endif
    }

    std::string readFile(const std::string &path) {
        std::ifstream file(path);
        if (!file)
            return "";

        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        return content;
    }
}

#endif // UTILS_H
