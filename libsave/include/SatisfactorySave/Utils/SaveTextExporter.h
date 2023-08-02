#pragma once

#include <filesystem>

#include "../GameTypes/Save/SaveGame.h"

namespace SatisfactorySave {

    void saveToTextFile(const SaveGame& savegame, const std::filesystem::path& filename);

} // namespace SatisfactorySave
