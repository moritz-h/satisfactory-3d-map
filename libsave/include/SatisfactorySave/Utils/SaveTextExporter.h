#pragma once

#include <filesystem>

#include "../GameTypes/Save/SaveGame.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    SATISFACTORYSAVE_API void saveToTextFile(const SaveGame& savegame, const std::filesystem::path& filename);

} // namespace SatisfactorySave
