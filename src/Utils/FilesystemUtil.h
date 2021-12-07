#ifndef SATISFACTORY3DMAP_FILESYSTEMUTIL_H
#define SATISFACTORY3DMAP_FILESYSTEMUTIL_H

#include <filesystem>
#include <vector>

namespace Satisfactory3DMap {
    std::vector<char> readFileToMemory(std::filesystem::path filename);

    std::filesystem::path getFullExeName();

    std::filesystem::path getExeDir();
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_FILESYSTEMUTIL_H
