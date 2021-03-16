#ifndef SATISFACTORY3DMAP_SAVEGAME_H
#define SATISFACTORY3DMAP_SAVEGAME_H

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Objects/SaveObjectBase.h"
#include "Types/SaveHeader.h"

namespace Satisfactory3DMap {

    class SaveGame {
    public:
        struct SaveNode {
            std::map<std::string, SaveNode> childNodes;
            std::map<std::string, std::shared_ptr<SaveObjectBase>> objects;
            std::size_t numObjects = 0;
            std::size_t numActors = 0;
        };

        explicit SaveGame(const std::filesystem::path& filepath);

        void printHeader() const;

        const std::vector<std::shared_ptr<SaveObjectBase>>& saveObjects() const {
            return save_objects_;
        }

        const SaveNode& root() const {
            return rootNode_;
        }

    protected:
        std::unique_ptr<SaveHeader> header_;
        std::vector<std::shared_ptr<SaveObjectBase>> save_objects_;
        std::vector<ObjectReference> collected_objects_;
        SaveNode rootNode_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_SAVEGAME_H
