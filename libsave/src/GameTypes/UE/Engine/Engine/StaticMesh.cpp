#include "GameTypes/UE/Engine/Engine/StaticMesh.h"

#include "GameTypes/FactoryGame/FGObjectReference.h"
#include "GameTypes/UE/Engine/EngineUtils.h"

// https://github.com/EpicGames/UnrealEngine/blob/5.3.2-release/Engine/Source/Runtime/Engine/Private/StaticMesh.cpp#L5534
void SatisfactorySave::UStaticMesh::serialize(Archive& ar) {
    if (!ar.isIArchive()) {
        throw std::runtime_error("Only IStreamArchive support implemented!");
    }

    UObject::serialize(ar);

    // UStaticMesh::Serialize

    // FStripDataFlags
    FStripDataFlags flags;
    ar << flags;
    flags.validateOnlyEditorDataIsStripped();

    // StaticMesh
    bool bCooked = false;
    ar << bCooked;

    FObjectReferenceDisc LocalBodySetup;
    ar << LocalBodySetup;

    FObjectReferenceDisc LocalNavCollision;
    ar << LocalNavCollision;

    FGuid LocalLightingGuid;
    ar << LocalLightingGuid;

    // TArray<class UStaticMeshSocket*> Sockets;
    int32_t SocketsNum = 0;
    ar << SocketsNum;
    if (SocketsNum != 0) {
        throw std::runtime_error("SocketsNum != 0 not implemented!");
    }

    ar << RenderData;

    bool bHasSpeedTreeWind = false;
    ar << bHasSpeedTreeWind;
    if (bHasSpeedTreeWind) {
        throw std::runtime_error("bHasSpeedTreeWind == true not implemented!");
    }

    ar << StaticMaterials;

    // CSS
    ar << MaxShadowLOD;
}
