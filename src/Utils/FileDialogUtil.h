#ifndef SATISFACTORY3DMAP_CONSOLEUTILS_H
#define SATISFACTORY3DMAP_CONSOLEUTILS_H

#include <filesystem>
#include <optional>

namespace Satisfactory3DMap {
    std::optional<std::filesystem::path> openFile();

    std::optional<std::filesystem::path> saveFile();
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_CONSOLEUTILS_H