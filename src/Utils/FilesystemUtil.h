#ifndef SATISFACTORY3DMAP_FILESYSTEMUTIL_H
#define SATISFACTORY3DMAP_FILESYSTEMUTIL_H

#include <filesystem>
#include <optional>
#include <vector>

namespace Satisfactory3DMap {
    std::vector<char> readFileToMemory(const std::filesystem::path& filename);

    std::filesystem::path getFullExePath();

    std::filesystem::path getExeDir();

    std::filesystem::path getConfigFile();

    std::filesystem::path getLogFile();

    std::optional<std::filesystem::path> findSteamExe();

    std::vector<std::filesystem::path> findSteamDirs();

    std::vector<std::filesystem::path> findGameDirs();
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_FILESYSTEMUTIL_H
