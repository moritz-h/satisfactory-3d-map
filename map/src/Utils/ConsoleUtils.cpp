#include "ConsoleUtils.h"

#ifdef _WIN32
#include <Windows.h>
#include <cstdio>
#endif

void Satisfactory3DMap::setConsoleUtf8() {
#ifdef _WIN32
    // Set console output to UTF-8, https://stackoverflow.com/a/45622802
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
#endif
}
