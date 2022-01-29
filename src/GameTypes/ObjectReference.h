#ifndef SATISFACTORY3DMAP_OBJECTREFERENCE_H
#define SATISFACTORY3DMAP_OBJECTREFERENCE_H

#include <string>
#include <utility>

#include "IO/Archive/Archive.h"

namespace Satisfactory3DMap {

    // FObjectReferenceDisc
    class ObjectReference {
    public:
        ObjectReference() = default;
        ObjectReference(std::string level_name, std::string path_name)
            : level_name_(std::move(level_name)),
              path_name_(std::move(path_name)){};

        void serialize(Archive& ar) {
            ar << level_name_;
            ar << path_name_;
        }

        [[nodiscard]] const std::string& levelName() const {
            return level_name_;
        }

        [[nodiscard]] const std::string& pathName() const {
            return path_name_;
        }

    private:
        std::string level_name_;
        std::string path_name_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_OBJECTREFERENCE_H
