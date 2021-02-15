#include <iostream>
#include <string>

#include "SaveGame/SaveGame.h"

#ifdef _WIN32
#include <Windows.h>
#include <cstdio>
#endif

int main(int argc, char* argv[]) {
    std::cout << "[SatisfactorySaveGame]" << std::endl;
    if (argc != 2) {
        std::cout << "Usage: SatisfactorySaveGame <filename>" << std::endl;
        return 1;
    }
    std::string filename(argv[1]);

#ifdef _WIN32
    // Set console output to UTF-8
    // https://stackoverflow.com/a/45622802
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
#endif

    try {
        SatisfactorySaveGame::SaveGame s(filename);
        s.printHeader();
    } catch (const std::exception& ex) {
        std::cerr << "Error occured: " << ex.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occured!" << std::endl;
        return 1;
    }

    return 0;
}
