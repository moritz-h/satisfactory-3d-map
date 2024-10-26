#pragma once

#include <deque>
#include <memory>
#include <string>
#include <vector>

#include <glad/gl.h>
#include <glowl/glowl.h>

#include "SatisfactorySave/GameTypes/Save/SaveGame.h"
#include "SatisfactorySave/Pak/PakManager.h"

#include "../Config/Configuration.h"
#include "../Config/PathSetting.h"
#include "ModelManager.h"

namespace Satisfactory3DMap {

    class DataView {
    public:
        struct ModelData {
            ModelData() : numActors(0) {};
            std::unique_ptr<glowl::BufferObject> listOffsetBuffer;
            int numActors;
        };
        struct SplineModelData {
            SplineModelData() : numInstances(0) {};
            std::unique_ptr<glowl::BufferObject> splineSegments;
            std::unique_ptr<glowl::BufferObject> instanceData;
            int numInstances;
        };

        explicit DataView(std::shared_ptr<Configuration> config);

        void openSave(const std::filesystem::path& file);
        void saveSave(const std::filesystem::path& file);

        [[nodiscard]] inline bool hasSave() const {
            return savegame_ != nullptr;
        }

        [[nodiscard]] inline const std::unique_ptr<SatisfactorySave::SaveGame>& saveGame() const {
            return savegame_;
        }

        void selectPathName(const std::string& pathName);

        void selectObject(int id) {
            if (savegame_ != nullptr && id >= 0 && id < static_cast<int>(savegame_->allSaveObjects().size())) {
                selectedObjectId_ = id;
                selectedObject_ = saveGame()->allSaveObjects().at(selectedObjectId_);
            } else {
                selectedObjectId_ = -1;
                selectedObject_ = nullptr;
            }
        }

        void selectObject(const std::shared_ptr<SatisfactorySave::SaveObject>& obj) {
            if (savegame_ != nullptr) {
                selectObject(savegame_->getGlobalId(obj));
            } else {
                selectObject(-1);
            }
        }

        [[nodiscard]] inline bool hasSelectedObject() const {
            return selectedObject_ != nullptr;
        }

        [[nodiscard]] inline int selectedObjectId() const {
            return selectedObjectId_;
        }

        [[nodiscard]] inline const std::shared_ptr<SatisfactorySave::SaveObject>& selectedObject() const {
            return selectedObject_;
        }

        void updateActor(int id, const SatisfactorySave::SaveObject& actor);

        [[nodiscard]] const std::shared_ptr<SatisfactorySave::PakManager>& pakManager() const {
            return pakManager_;
        }

        // TODO remove from interface
        [[nodiscard]] const std::unique_ptr<ModelManager>& manager() const {
            return manager_;
        }

        [[nodiscard]] const std::unique_ptr<glowl::BufferObject>& actorIdBuffer() const {
            return actorIdBuffer_;
        }

        [[nodiscard]] const std::unique_ptr<glowl::BufferObject>& actorTransformationBuffer() const {
            return actorTransformationBuffer_;
        }

        [[nodiscard]] const std::vector<ModelData>& pakModelDataList() const {
            return pakModelDataList_;
        }

        [[nodiscard]] const std::vector<ModelData>& modelDataList() const {
            return modelDataList_;
        }

        [[nodiscard]] const std::vector<SplineModelData>& splineModelDataList() const {
            return splineModelDataList_;
        }

        std::deque<std::string>& showErrors() {
            return showErrors_;
        }

    protected:
        std::shared_ptr<Configuration> config_;
        std::shared_ptr<PathSetting> gameDirSetting_;

        std::shared_ptr<SatisfactorySave::PakManager> pakManager_;
        std::unique_ptr<ModelManager> manager_;
        std::unique_ptr<SatisfactorySave::SaveGame> savegame_;

        int selectedObjectId_;
        std::shared_ptr<SatisfactorySave::SaveObject> selectedObject_;

        std::unique_ptr<glowl::BufferObject> actorIdBuffer_;
        std::unique_ptr<glowl::BufferObject> actorTransformationBuffer_;

        std::vector<ModelData> pakModelDataList_;
        std::vector<ModelData> modelDataList_;
        std::vector<SplineModelData> splineModelDataList_;

        std::unordered_map<int32_t, int32_t> actorBufferPositions_;

        std::deque<std::string> showErrors_;
    };
} // namespace Satisfactory3DMap
