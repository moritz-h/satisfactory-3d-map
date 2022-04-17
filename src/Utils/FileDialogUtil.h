#pragma once

#include <filesystem>
#include <optional>

namespace Satisfactory3DMap {
    std::optional<std::filesystem::path> openFile();

    std::optional<std::filesystem::path> saveFile();
} // namespace Satisfactory3DMap
