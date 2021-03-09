#ifndef SATISFACTORY3DMAP_SAVEGAME_H
#define SATISFACTORY3DMAP_SAVEGAME_H

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "Objects/SaveObjectBase.h"
#include "Types/SaveHeader.h"

namespace Satisfactory3DMap {

    class SaveGame {
    public:
        explicit SaveGame(const std::filesystem::path& filepath);

        void printHeader() const;

    protected:
        std::unique_ptr<SaveHeader> header_;
        std::vector<std::shared_ptr<SaveObjectBase>> save_objects_;
        std::vector<ObjectReference> collected_objects_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_SAVEGAME_H
