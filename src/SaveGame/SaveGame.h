#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "GameTypes/Save/ChunkHeader.h"
#include "GameTypes/Save/SaveHeader.h"
#include "GameTypes/SaveObjects/SaveObjectBase.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

namespace Satisfactory3DMap {

    class SaveGame {
    public:
        typedef std::shared_ptr<SaveObjectBase> SaveObjectPtr;
        typedef std::vector<SaveObjectPtr> SaveObjectList;

        struct LevelData {
            std::string level_name;
            SaveObjectList save_objects;
            std::vector<ObjectReference> destroyed_actors_TOC;
            std::vector<ObjectReference> destroyed_actors;
        };

        struct SaveNode {
            std::map<std::string, SaveNode> childNodes;
            std::map<std::string, std::shared_ptr<SaveObjectBase>> objects;
            std::size_t numObjects = 0;
            std::size_t numActors = 0;
        };

        explicit SaveGame(const std::filesystem::path& filepath);

        void save(const std::filesystem::path& filepath);

        [[nodiscard]] const SaveHeader& header() const {
            return header_;
        }

        [[nodiscard]] const std::vector<LevelData>& levelData() const {
            return level_data_;
        }

        [[nodiscard]] const std::vector<std::shared_ptr<SaveObjectBase>>& saveObjects() const {
            return save_objects_;
        }

        [[nodiscard]] const std::vector<ObjectReference>& destroyedActors() const {
            return destroyed_actors_toc_;
        }

        [[nodiscard]] const std::vector<ObjectReference>& unresolvedWorldSaveData() const {
            return unresolved_world_save_data_;
        }

        const std::shared_ptr<SaveObjectBase>& getObjectByPath(const std::string& path) {
            return path_object_map_.at(path);
        }

        [[nodiscard]] const std::vector<SaveNode>& levelRootNodes() const {
            return levelRootNodes_;
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

        static void parseTOCBlob(IStreamArchive& ar, SaveObjectList& saveObjects,
            std::vector<ObjectReference>& destroyedActorsTOC);

        static void parseDataBlob(IStreamArchive& ar, SaveObjectList& saveObjects);

        void initAccessStructures(const SaveObjectList& saveObjects, SaveNode& rootNode);

        static void saveTOCBlob(OStreamArchive& ar, SaveObjectList& saveObjects,
            std::vector<ObjectReference>& destroyedActorsTOC);

        static void saveDataBlob(OStreamArchive& ar, SaveObjectList& saveObjects);

        SaveHeader header_;
        std::vector<LevelData> level_data_;
        SaveObjectList save_objects_;
        std::vector<ObjectReference> destroyed_actors_toc_;
        std::vector<ObjectReference> unresolved_world_save_data_;

        std::unordered_map<std::string, std::shared_ptr<SaveObjectBase>> path_object_map_;
        std::vector<SaveNode> levelRootNodes_;
        SaveNode rootNode_;
    };
} // namespace Satisfactory3DMap
