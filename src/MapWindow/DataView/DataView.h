#ifndef SATISFACTORY3DMAP_DATAVIEW_H
#define SATISFACTORY3DMAP_DATAVIEW_H

#include <deque>
#include <memory>
#include <string>
#include <vector>

#include <glad/gl.h>
#include <glowl/glowl.h>

#include "IO/Pak/PakFile.h"
#include "ModelManager.h"
#include "SaveGame/SaveGame.h"

namespace Satisfactory3DMap {

    class DataView {
    public:
        struct ModelData {
            ModelData() : numActors(0){};
            std::unique_ptr<glowl::BufferObject> listOffsetBuffer;
            int numActors;
        };
        struct SplineModelData {
            SplineModelData() : numInstances(0){};
            std::unique_ptr<glowl::BufferObject> splineSegments;
            std::unique_ptr<glowl::BufferObject> instanceData;
            int numInstances;
        };

        DataView();

        void openSave(const std::filesystem::path& file);
        void saveSave(const std::filesystem::path& file);

        [[nodiscard]] inline bool hasSave() const {
            return savegame_ != nullptr;
        }

        [[nodiscard]] inline const std::unique_ptr<SaveGame>& saveGame() const {
            return savegame_;
        }

        void selectPathName(const std::string& pathName);

        void selectObject(int id) {
            selectedObjectId_ = id;
        }

        [[nodiscard]] inline bool hasSelectedObject() const {
            return selectedObjectId_ >= 0 && selectedObjectId_ < static_cast<int>(savegame_->saveObjects().size());
        }

        [[nodiscard]] inline int selectedObjectId() const {
            return selectedObjectId_;
        }

        [[nodiscard]] inline const std::shared_ptr<SaveObjectBase>& selectedObject() const {
            return saveGame()->saveObjects().at(selectedObjectId_);
        }

        void updateActor(const SaveActor& actor);

        // TODO remove from interface
        [[nodiscard]] const std::unique_ptr<ModelManager>& manager() const {
            return manager_;
        }

        [[nodiscard]] const std::shared_ptr<PakFile>& pak() const {
            return pak_;
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
        std::unique_ptr<ModelManager> manager_;

        std::shared_ptr<PakFile> pak_;
        std::unique_ptr<SaveGame> savegame_;

        int selectedObjectId_;

        std::unique_ptr<glowl::BufferObject> actorIdBuffer_;
        std::unique_ptr<glowl::BufferObject> actorTransformationBuffer_;

        std::vector<ModelData> pakModelDataList_;
        std::vector<ModelData> modelDataList_;
        std::vector<SplineModelData> splineModelDataList_;

        std::unordered_map<int32_t, int32_t> actorBufferPositions_;

        std::deque<std::string> showErrors_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_DATAVIEW_H
