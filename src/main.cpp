#include <iostream>
#include <string>

#include "SaveGame/SaveGame.h"

int main(int argc, char* argv[]) {
    std::cout << "[SatisfactorySaveGame]" << std::endl;
    if (argc != 2) {
        std::cout << "Usage: SatisfactorySaveGame <filename>" << std::endl;
        return 1;
    }
    std::string filename(argv[1]);

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
