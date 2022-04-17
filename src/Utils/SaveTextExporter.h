#pragma once

#include <string>

#include "SaveGame/SaveGame.h"

namespace Satisfactory3DMap {
    void saveToTextFile(const SaveGame& savegame, const std::string& filename);

} // namespace Satisfactory3DMap
