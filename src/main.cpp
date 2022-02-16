#include <filesystem>
#include <iostream>

#include "MapWindow/MapWindow.h"
#include "Utils/ConsoleUtils.h"

int main(int argc, char* argv[]) {
    Satisfactory3DMap::setConsoleUtf8();

    std::cout << "[Satisfactory3DMap]" << std::endl;
    if (argc > 2) {
        std::cout << "Usage: Satisfactory3DMap [filename]" << std::endl;
        return 1;
    }
    std::filesystem::path savefile;
    if (argc == 2) {
        savefile = std::filesystem::path(argv[1]);
    }

    try {
        Satisfactory3DMap::MapWindow window;
        window.dataView()->openSave(savefile);
        window.run();
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Exception: Unknown error!" << std::endl;
        return 1;
    }

    return 0;
}
