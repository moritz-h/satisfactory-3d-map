#ifndef SATISFACTORY3DMAP_SAVEOBJECT_H
#define SATISFACTORY3DMAP_SAVEOBJECT_H

#include "SaveObjectBase.h"

namespace Satisfactory3DMap {

    // FObjectSaveHeader
    class SaveObject : public SaveObjectBase {
    public:
        explicit SaveObject(int32_t id);

        void serialize(Archive& ar) override;

        [[nodiscard]] const std::string& outerPathName() const {
            return outer_path_name_;
        }

    protected:
        std::string outer_path_name_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_SAVEOBJECT_H
