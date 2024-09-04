#include "Pak/Serialization/StaticMesh.h"

// https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Engine/Private/StaticMesh.cpp#L4554
void SatisfactorySave::StaticMesh::serialize(Archive& ar) {

    // From UObject::Serialize

    ar << properties_;
    ar << guid_;

    // UStaticMesh::Serialize

    // FStripDataFlags
    FStripDataFlags flags;
    ar << flags;
    flags.validateOnlyEditorDataIsStripped();

    // StaticMesh
    bool bCooked = false;
    ar << bCooked;

    int32_t LocalBodySetup = 0;
    ar << LocalBodySetup; // TODO object reference

    int32_t LocalNavCollision = 0;
    ar << LocalNavCollision; // TODO object reference

    FGuid LocalLightingGuid;
    ar << LocalLightingGuid;

    // TArray<class UStaticMeshSocket*> Sockets;
    int32_t SocketsNum = 0;
    ar << SocketsNum;
    if (SocketsNum != 0) {
        throw std::runtime_error("SocketsNum != 0 not implemented!");
    }

    ar << RenderData;

    // TODO
}
