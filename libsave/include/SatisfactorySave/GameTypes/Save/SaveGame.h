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
        struct PerLevelData {
            std::string level_name;
            SaveObjectBaseList save_objects;
            bool has_destroyed_actors_TOC = false;
            std::vector<FObjectReferenceDisc> destroyed_actors_TOC;
            std::vector<FObjectReferenceDisc> destroyed_actors;
        };

        struct PersistentAndRuntimeData {
            SaveObjectBaseList save_objects;
            std::vector<FObjectReferenceDisc> destroyed_actors_TOC;
            // TMap<FString, TArray<FObjectReferenceDisc>> LevelToDestroyedActorsMap; // is always zero.
        };

        struct SaveNode {
            std::map<std::string, SaveNode> childNodes;
            SaveObjectBaseList objects;
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

        [[nodiscard]] const SaveObjectBaseList& allSaveObjects() const {
            return all_save_objects_;
        }

        [[nodiscard]] inline bool isObjectPath(const std::string& path) const {
            return path_objects_map_.contains(path);
        }

        const SaveObjectBaseList& getObjectsByPath(const std::string& path) {
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

        [[nodiscard]] int32_t getGlobalId(const SaveObjectBasePtr& obj) {
            auto it = info_map_.find(obj);
            if (it != info_map_.end()) {
                // TODO handle id size cast
                return static_cast<int32_t>(it->second.all_list_idx);
            }
            return -1;
        }

        bool addObject(const SaveObjectBasePtr& obj, int level = -1);
        bool addObjects(const SaveObjectBaseList& objects, int level = -1);

        bool removeObject(const SaveObjectBasePtr& obj);
        bool removeObjects(const SaveObjectBaseList& objects);

    protected:
        struct SaveObjectInfo {
            int level_idx = -1;
            std::size_t level_list_idx = 0;
            std::size_t all_list_idx = 0;
        };

        static void parseTOCBlob(IStreamArchive& ar, SaveObjectBaseList& saveObjects,
            std::vector<FObjectReferenceDisc>& destroyedActorsTOC, bool& has_destroyedActorsTOC);

        static void parseDataBlob(IStreamArchive& ar, SaveObjectBaseList& saveObjects);

        void initAccessStructures();

        void initAccessStructures(const SaveObjectBaseList& saveObjects, SaveNode& rootNode);

        static void saveTOCBlob(OStreamArchive& ar, SaveObjectBaseList& saveObjects,
            std::vector<FObjectReferenceDisc>& destroyedActorsTOC, bool has_destroyedActorsTOC);

        static void saveDataBlob(OStreamArchive& ar, SaveObjectBaseList& saveObjects);

        // Save data
        FSaveHeader header_;

        FWorldPartitionValidationData ValidationData;

        std::vector<PerLevelData> per_level_data_;
        PersistentAndRuntimeData persistent_and_runtime_data_;
        std::vector<FObjectReferenceDisc> unresolved_world_save_data_;

        // Redundant structures for object access
        SaveObjectBaseList all_save_objects_;
        std::unordered_map<SaveObjectBasePtr, SaveObjectInfo> info_map_;
        std::unordered_map<std::string, SaveObjectBaseList> path_objects_map_;
        std::vector<SaveNode> level_root_nodes_;
        SaveNode persistent_and_runtime_root_node_;
        SaveNode all_root_node_;
    };
} // namespace SatisfactorySave
