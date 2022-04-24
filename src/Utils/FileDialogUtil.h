#pragma once

#include <filesystem>
#include <optional>

namespace Satisfactory3DMap {
    std::optional<std::filesystem::path> openFile();

    std::optional<std::filesystem::path> saveFile();

    std::optional<std::filesystem::path> selectFolder();
} // namespace Satisfactory3DMap
