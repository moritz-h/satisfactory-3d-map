#ifndef SATISFACTORYSAVEGAME_SAVEACTOR_H
#define SATISFACTORYSAVEGAME_SAVEACTOR_H

#include <vector>

#include "SaveObjectBase.h"
#include "Vectors.h"

namespace SatisfactorySaveGame {

    // FActorSaveHeader
    class SaveActor : public SaveObjectBase {
    public:
        SaveActor(int32_t type, std::istream& stream);

        void parseData(int32_t length, std::istream& stream) override;

    protected:
        int32_t need_transform_;
        Vec4 rotation_;
        Vec3 position_;
        Vec3 scale_;
        int32_t was_placed_in_level_;

        std::unique_ptr<ObjectReference> parent_reference_;
        std::unique_ptr<std::vector<ObjectReference>> child_references_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_SAVEACTOR_H
