#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
    void clearScreen();
    void sleep(int milliseconds);
    void moveCursor(int x, int y);
    std::string readFile(const std::string& path);
}

#endif
