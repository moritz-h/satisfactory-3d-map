#include "Paks.h"

namespace {
    std::filesystem::path findGameDirectory() {
        // TODO
        std::filesystem::path gameDir(R"(C:\Program Files (x86)\Steam\steamapps\common\Satisfactory)");
        return gameDir;
    }

    const std::filesystem::path mainPakPath("FactoryGame/Content/Paks/FactoryGame-WindowsNoEditor.pak");
} // namespace

Satisfactory3DMap::PakFile Satisfactory3DMap::Paks::getMainPakFile() {
    std::filesystem::path pakPath = findGameDirectory() / mainPakPath;
    if (!std::filesystem::is_regular_file(pakPath)) {
        throw std::runtime_error("Pak file not found!");
    }
    pakPath = std::filesystem::canonical(pakPath);

    return PakFile(pakPath);
}
