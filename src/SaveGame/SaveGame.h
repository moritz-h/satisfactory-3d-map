#ifndef SATISFACTORYSAVEGAME_SAVEGAME_H
#define SATISFACTORYSAVEGAME_SAVEGAME_H

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "Objects/SaveObjectBase.h"
#include "Types/SaveHeader.h"

namespace SatisfactorySaveGame {

    class SaveGame {
    public:
        explicit SaveGame(const std::filesystem::path& filepath);

        void printHeader() const;

    protected:
        std::unique_ptr<SaveHeader> header_;
        std::vector<std::shared_ptr<SaveObjectBase>> save_objects_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_SAVEGAME_H
