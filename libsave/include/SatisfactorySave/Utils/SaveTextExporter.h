#pragma once

#include <filesystem>

#include "../SaveGame.h"

namespace SatisfactorySave {

    void saveToTextFile(const SaveGame& savegame, const std::filesystem::path& filename);

} // namespace SatisfactorySave
