#ifndef SATISFACTORY3DMAP_SAVEGAME_H
#define SATISFACTORY3DMAP_SAVEGAME_H

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Objects/SaveObjectBase.h"
#include "Types/ChunkHeader.h"
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

        void save(const std::filesystem::path& filepath);

        [[nodiscard]] const std::unique_ptr<SaveHeader>& header() const {
            return header_;
        }

        [[nodiscard]] const std::vector<std::shared_ptr<SaveObjectBase>>& saveObjects() const {
            return save_objects_;
        }

        [[nodiscard]] const std::vector<ObjectReference>& collectedObjects() const {
            return collected_objects_;
        }

        const std::shared_ptr<SaveObjectBase>& getObjectByPath(const std::string& path) {
            return path_object_map_.at(path);
        }

        [[nodiscard]] const SaveNode& root() const {
            return rootNode_;
        }

    protected:
        struct ChunkInfo {
            ChunkInfo(ChunkHeader header, std::vector<char> compressed_chunk, std::size_t decompressed_offset)
                : header(header),
                  compressed_chunk(std::move(compressed_chunk)),
                  decompressed_offset(decompressed_offset){};
            ChunkHeader header;
            std::vector<char> compressed_chunk;
            std::size_t decompressed_offset;
        };

        std::unique_ptr<SaveHeader> header_;
        std::vector<std::shared_ptr<SaveObjectBase>> save_objects_;
        std::vector<ObjectReference> collected_objects_;

        std::unordered_map<std::string, std::shared_ptr<SaveObjectBase>> path_object_map_;
        SaveNode rootNode_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_SAVEGAME_H
