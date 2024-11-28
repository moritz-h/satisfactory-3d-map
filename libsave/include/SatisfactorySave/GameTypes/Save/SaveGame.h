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

        explicit SaveGame(const std::filesystem::path& filepath);

        void save(const std::filesystem::path& filepath);

        [[nodiscard]] inline const SaveObjectList& allSaveObjects() const {
            return all_save_objects_;
        }

        [[nodiscard]] inline bool isObjectClass(const std::string& className) const {
            return class_objects_map_.contains(className);
        }

        [[nodiscard]] inline const SaveObjectList& getObjectsByClass(const std::string& className) const {
            return class_objects_map_.at(className);
        }

        [[nodiscard]] inline bool isObjectPath(const std::string& path) const {
            return path_objects_map_.contains(path);
        }

        [[nodiscard]] inline const SaveObjectList& getObjectsByPath(const std::string& path) {
            return path_objects_map_.at(path);
        }

        bool addObject(const SaveObjectPtr& obj, int level = -1);
        bool addObjects(const SaveObjectList& objects, int level = -1);

        bool removeObject(const SaveObjectPtr& obj);
        bool removeObjects(const SaveObjectList& objects);

    protected:
        void initAccessStructures();
        void addObjectAccess(const SaveObjectPtr& obj, int level = -1);

        // Redundant structures for object access
        SaveObjectList all_save_objects_;
        std::unordered_map<std::string, SaveObjectList> class_objects_map_;
        std::unordered_map<std::string, SaveObjectList> path_objects_map_;
        std::unordered_map<SaveObjectPtr, int> object_level_map_;
    };
} // namespace SatisfactorySave
