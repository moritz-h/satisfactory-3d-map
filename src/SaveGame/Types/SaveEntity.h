#ifndef SATISFACTORYSAVEGAME_SAVEENTITY_H
#define SATISFACTORYSAVEGAME_SAVEENTITY_H

#include "SaveObject.h"
#include "Vectors.h"

namespace SatisfactorySaveGame {

    class SaveEntity : public SaveObject {
    public:
        SaveEntity(int32_t type, std::istream& stream);

    private:
        int32_t need_transform_;
        Vec4 rotation_;
        Vec3 position_;
        Vec3 scale_;
        int32_t was_placed_in_level_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_SAVEENTITY_H
