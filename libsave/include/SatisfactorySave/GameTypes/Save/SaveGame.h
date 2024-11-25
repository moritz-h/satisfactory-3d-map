#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../FactoryGame/FGSaveManagerInterface.h"
#include "../FactoryGame/FGSaveSession.h"
#include "../FactoryGame/FWPSaveDataMigrationContext.h"
#include "SaveObject.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API SaveGame {
    public:
        // Save data
        FSaveHeader mSaveHeader;
        FWorldPartitionValidationData SaveGameValidationData;
        TMap<std::string, FPerStreamingLevelSaveData> mPerLevelDataMap;
        FPersistentAndRuntimeSaveData mPersistentAndRuntimeData;
        FUnresolvedWorldSaveData mUnresolvedWorldSaveData;

        struct SaveNode {
            std::map<std::string, SaveNode> childNodes;
            SaveObjectList objects;
            std::size_t numObjects = 0;
            std::size_t numActors = 0;
        };

        explicit SaveGame(const std::filesystem::path& filepath);

        void save(const std::filesystem::path& filepath);

        [[nodiscard]] const SaveObjectList& allSaveObjects() const {
            return all_save_objects_;
        }

        [[nodiscard]] inline bool isObjectPath(const std::string& path) const {
            return path_objects_map_.contains(path);
        }

        const SaveObjectList& getObjectsByPath(const std::string& path) {
            return path_objects_map_.at(path);
        }

        SaveObjectList getObjectsByClass(const std::string& className);

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

        void initAccessStructures();

        void initAccessStructures(const SaveObjectList& saveObjects, SaveNode& rootNode);

        // Redundant structures for object access
        SaveObjectList all_save_objects_;
        std::unordered_map<SaveObjectPtr, SaveObjectInfo> info_map_;
        std::unordered_map<std::string, SaveObjectList> path_objects_map_;
        std::vector<SaveNode> level_root_nodes_;
        SaveNode persistent_and_runtime_root_node_;
        SaveNode all_root_node_;
    };
} // namespace SatisfactorySave
