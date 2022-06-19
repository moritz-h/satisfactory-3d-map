#pragma once

#include <filesystem>

#include "SaveGame/SaveGame.h"

namespace Satisfactory3DMap {
    void saveToTextFile(const SaveGame& savegame, const std::filesystem::path& filename);

} // namespace Satisfactory3DMap
