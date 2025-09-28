#pragma once

#include <cstdint>
#include <vector>

#include "../../../../IO/Archive/Archive.h"
#include "../../Core/Math/Vector.h"
#include "../../Core/Math/Vector4.h"
#include "../../CoreUObject/Serialization/BulkData.h"
#include "satisfactorysave_export.h"

#define NANITE_MAX_BVH_NODE_FANOUT_BITS 2
#define NANITE_MAX_BVH_NODE_FANOUT (1 << NANITE_MAX_BVH_NODE_FANOUT_BITS)

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FPackedHierarchyNode {
        FVector4f LODBounds[NANITE_MAX_BVH_NODE_FANOUT];

        struct {
            FVector3f BoxBoundsCenter;
            uint32_t MinLODError_MaxParentLODError = 0;
        } Misc0[NANITE_MAX_BVH_NODE_FANOUT];

        struct {
            FVector3f BoxBoundsExtent;
            uint32_t ChildStartReference = 0;
        } Misc1[NANITE_MAX_BVH_NODE_FANOUT];

        struct {
            uint32_t ResourcePageIndex_NumPages_GroupPartSize = 0;
        } Misc2[NANITE_MAX_BVH_NODE_FANOUT];

        void serialize(Archive& ar) {
            for (uint32_t i = 0; i < NANITE_MAX_BVH_NODE_FANOUT; i++) {
                ar << LODBounds[i];
                ar << Misc0[i].BoxBoundsCenter;
                ar << Misc0[i].MinLODError_MaxParentLODError;
                ar << Misc1[i].BoxBoundsExtent;
                ar << Misc1[i].ChildStartReference;
                ar << Misc2[i].ResourcePageIndex_NumPages_GroupPartSize;
            }
        }
    };

    struct SATISFACTORYSAVE_API FPageStreamingState {
        uint32_t BulkOffset;
        uint32_t BulkSize;
        uint32_t PageSize;
        uint32_t DependenciesStart;
        uint16_t DependenciesNum;
        uint8_t MaxHierarchyDepth;
        uint8_t Flags;

        void serialize(Archive& ar) {
            ar << BulkOffset;
            ar << BulkSize;
            ar << PageSize;
            ar << DependenciesStart;
            ar << DependenciesNum;
            ar << MaxHierarchyDepth;
            ar << Flags;
        }
    };

    struct SATISFACTORYSAVE_API FResources {
        std::vector<uint8_t> RootData;
        FByteBulkData StreamablePages;
        std::vector<uint16_t> ImposterAtlas;
        std::vector<FPackedHierarchyNode> HierarchyNodes;
        std::vector<uint32_t> HierarchyRootOffsets;
        std::vector<FPageStreamingState> PageStreamingStates;
        std::vector<uint32_t> PageDependencies;
        uint32_t NumRootPages = 0;
        int32_t PositionPrecision = 0;
        int32_t NormalPrecision = 0;
        uint32_t NumInputTriangles = 0;
        uint32_t NumInputVertices = 0;
        uint16_t NumInputMeshes = 0;
        uint16_t NumInputTexCoords = 0;
        uint32_t NumClusters = 0;
        uint32_t ResourceFlags = 0;

        void serialize(Archive& ar) {
            // https://github.com/EpicGames/UnrealEngine/blob/5.3.2-release/Engine/Source/Runtime/Engine/Private/Rendering/NaniteResources.cpp#L233

            // FStripDataFlags
            FStripDataFlags flags;
            ar << flags;
            flags.validateOnlyEditorDataIsStripped();

            ar << ResourceFlags;
            ar << StreamablePages;
            ar << RootData;
            ar << PageStreamingStates;
            ar << HierarchyNodes;
            ar << HierarchyRootOffsets;
            ar << PageDependencies;
            ar << ImposterAtlas;
            ar << NumRootPages;
            ar << PositionPrecision;
            ar << NormalPrecision;
            ar << NumInputTriangles;
            ar << NumInputVertices;
            ar << NumInputMeshes;
            ar << NumInputTexCoords;
            ar << NumClusters;
        }
    };
} // namespace SatisfactorySave
