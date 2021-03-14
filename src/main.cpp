#include <iostream>
#include <string>

#include "MapWindow/MapWindow.h"
#include "Utils/ConsoleUtils.h"

int main(int argc, char* argv[]) {
    Satisfactory3DMap::setConsoleUtf8();

    std::cout << "[Satisfactory3DMap]" << std::endl;
    if (argc > 2) {
        std::cout << "Usage: Satisfactory3DMap [filename]" << std::endl;
        return 1;
    }
    std::string filename;
    if (argc == 2) {
        filename = std::string(argv[1]);
    }

    try {
        Satisfactory3DMap::MapWindow window;
        window.openSave(filename);
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
