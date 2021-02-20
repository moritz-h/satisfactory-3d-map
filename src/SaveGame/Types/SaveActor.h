#ifndef SATISFACTORYSAVEGAME_SAVEACTOR_H
#define SATISFACTORYSAVEGAME_SAVEACTOR_H

#include "SaveObjectBase.h"
#include "Vectors.h"

namespace SatisfactorySaveGame {

    // FActorSaveHeader
    class SaveActor : public SaveObjectBase {
    public:
        SaveActor(int32_t type, std::istream& stream);

    private:
        int32_t need_transform_;
        Vec4 rotation_;
        Vec3 position_;
        Vec3 scale_;
        int32_t was_placed_in_level_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_SAVEACTOR_H
