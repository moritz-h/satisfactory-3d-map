#include "StaticMesh.h"

// https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Engine/Private/StaticMesh.cpp#L4554
void Satisfactory3DMap::StaticMesh::serialize(Archive& ar) {

    // From UObject::Serialize

    ar << properties_;

    ar << hasGuid_;
    if (hasGuid_) {
        ar << guid_;
    }

    // UStaticMesh::Serialize

    // FStripDataFlags
    uint8_t GlobalStripFlags = 0;
    uint8_t ClassStripFlags = 0;
    ar << GlobalStripFlags;
    ar << ClassStripFlags;

    // StaticMesh
    bool bCooked = false;
    ar << bCooked;

    // TODO
}
