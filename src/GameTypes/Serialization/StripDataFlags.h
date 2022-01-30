#ifndef SATISFACTORY3DMAP_STRIPDATAFLAGS_H
#define SATISFACTORY3DMAP_STRIPDATAFLAGS_H

#include "IO/Archive/Archive.h"

namespace Satisfactory3DMap {

    struct FStripDataFlags {
        uint8_t GlobalStripFlags = 0;
        uint8_t ClassStripFlags = 0;

        void serialize(Archive& ar) {
            ar << GlobalStripFlags;
            ar << ClassStripFlags;
        }
    };

} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRIPDATAFLAGS_H
