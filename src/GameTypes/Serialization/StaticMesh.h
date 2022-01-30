#ifndef SATISFACTORY3DMAP_STATICMESH_H
#define SATISFACTORY3DMAP_STATICMESH_H

#include "../Guid.h"
#include "../Properties/Properties.h"
#include "IO/Archive/Archive.h"

namespace Satisfactory3DMap {

    // FStaticMesh
    class StaticMesh {
    public:
        StaticMesh() = default;

        void serialize(Archive& ar);

    protected:
        Properties properties_;
        bool hasGuid_ = false;
        Guid guid_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STATICMESH_H
