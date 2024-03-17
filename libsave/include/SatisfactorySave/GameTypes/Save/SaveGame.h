#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../IO/Archive/IStreamArchive.h"
#include "../../IO/Archive/OStreamArchive.h"
#include "../UE/Satisfactory/SaveDataMigrationContext.h"
#include "../UE/Satisfactory/SaveHeader.h"
#include "SaveObjectBase.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API SaveGame {
    public:
        typedef std::shared_ptr<SaveObjectBase> SaveObjectPtr;
        typedef std::vector<SaveObjectPtr> SaveObjectList;

        struct PerLevelData {
            std::string level_name;
            SaveObjectList save_objects;
            bool has_destroyed_actors_TOC = false;
            std::vector<FObjectReferenceDisc> destroyed_actors_TOC;
            std::vector<FObjectReferenceDisc> destroyed_actors;
        };

        struct PersistentAndRuntimeData {
            SaveObjectList save_objects;
            std::vector<FObjectReferenceDisc> destroyed_actors_TOC;
            // TMap<FString, TArray<FObjectReferenceDisc>> LevelToDestroyedActorsMap; // is always zero.
        };

        struct SaveNode {
            std::map<std::string, SaveNode> childNodes;
            SaveObjectList objects;
            std::size_t numObjects = 0;
            std::size_t numActors = 0;
        };

        explicit SaveGame(const std::filesystem::path& filepath);

        void save(const std::filesystem::path& filepath);

        [[nodiscard]] const FSaveHeader& header() const {
            return header_;
        }

        [[nodiscard]] const FWorldPartitionValidationData& validationData() const {
            return ValidationData;
        }

        [[nodiscard]] const std::vector<PerLevelData>& perLevelData() const {
            return per_level_data_;
        }

        [[nodiscard]] const PersistentAndRuntimeData& persistentAndRuntimeData() const {
            return persistent_and_runtime_data_;
        }

        [[nodiscard]] const std::vector<FObjectReferenceDisc>& unresolvedWorldSaveData() const {
            return unresolved_world_save_data_;
        }

        [[nodiscard]] const std::vector<std::shared_ptr<SaveObjectBase>>& allSaveObjects() const {
            return all_save_objects_;
        }

        [[nodiscard]] inline bool isObjectPath(const std::string& path) const {
            return path_objects_map_.contains(path);
        }

        const SaveObjectList& getObjectsByPath(const std::string& path) {
            return path_objects_map_.at(path);
        }

        [[nodiscard]] const std::vector<SaveNode>& levelRootNodes() const {
            return level_root_nodes_;
        }

        [[nodiscard]] const SaveNode& persistentAndRuntimeRootNode() const {
            return persistent_and_runtime_root_node_;
        }

        [[nodiscard]] const SaveNode& allRootNode() const {
            return all_root_node_;
        }

        [[nodiscard]] int32_t getGlobalId(const SaveObjectPtr& obj) {
            auto it = info_map_.find(obj);
            if (it != info_map_.end()) {
                // TODO handle id size cast
                return static_cast<int32_t>(it->second.all_list_idx);
            }
            return -1;
        }

        bool addObject(const SaveObjectPtr& obj, int level = -1);
        bool addObjects(const SaveObjectList& objects, int level = -1);

        bool removeObject(const SaveObjectPtr& obj);
        bool removeObjects(const SaveObjectList& objects);

    protected:
        struct SaveObjectInfo {
            int level_idx = -1;
            std::size_t level_list_idx = 0;
            std::size_t all_list_idx = 0;
        };

        static void parseTOCBlob(IStreamArchive& ar, SaveObjectList& saveObjects,
            std::vector<FObjectReferenceDisc>& destroyedActorsTOC, bool& has_destroyedActorsTOC);

        static void parseDataBlob(IStreamArchive& ar, SaveObjectList& saveObjects);

        void initAccessStructures();

        void initAccessStructures(const SaveObjectList& saveObjects, SaveNode& rootNode);

        static void saveTOCBlob(OStreamArchive& ar, SaveObjectList& saveObjects,
            std::vector<FObjectReferenceDisc>& destroyedActorsTOC, bool has_destroyedActorsTOC);

        static void saveDataBlob(OStreamArchive& ar, SaveObjectList& saveObjects);

        // Save data
        FSaveHeader header_;

        FWorldPartitionValidationData ValidationData;

        std::vector<PerLevelData> per_level_data_;
        PersistentAndRuntimeData persistent_and_runtime_data_;
        std::vector<FObjectReferenceDisc> unresolved_world_save_data_;

        // Redundant structures for object access
        SaveObjectList all_save_objects_;
        std::unordered_map<SaveObjectPtr, SaveObjectInfo> info_map_;
        std::unordered_map<std::string, SaveObjectList> path_objects_map_;
        std::vector<SaveNode> level_root_nodes_;
        SaveNode persistent_and_runtime_root_node_;
        SaveNode all_root_node_;
    };
} // namespace SatisfactorySave
