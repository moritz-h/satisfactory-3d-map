#pragma once

#include <optional>
#include <vector>

#include "../../GameTypes/Properties/Base/PropertyList.h"
#include "../../GameTypes/UE/Core/Misc/Guid.h"
#include "../../IO/Archive/Archive.h"
#include "StripDataFlags.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FStaticMeshSection {
        int32_t MaterialIndex = 0;
        uint32_t FirstIndex = 0;
        uint32_t NumTriangles = 0;
        uint32_t MinVertexIndex = 0;
        uint32_t MaxVertexIndex = 0;
        bool bEnableCollision = false;
        bool bCastShadow = false;
        bool bForceOpaque = false;
        bool bVisibleInRayTracing = false;
        bool bAffectDistanceFieldLighting = false;

        void serialize(Archive& ar) {
            ar << MaterialIndex;
            ar << FirstIndex;
            ar << NumTriangles;
            ar << MinVertexIndex;
            ar << MaxVertexIndex;
            ar << bEnableCollision;
            ar << bCastShadow;
            ar << bForceOpaque;
            ar << bVisibleInRayTracing;
            ar << bAffectDistanceFieldLighting;
        }
    };

    // TResourceArray
    struct SATISFACTORYSAVE_API ResourceArray {
        int32_t SerializedElementSize = 0;
        int32_t Num = 0;
        std::vector<char> data;

        void serialize(Archive& ar) {
            // BulkSerialize
            ar << SerializedElementSize;
            ar << Num;

            auto& inAr = dynamic_cast<IStreamArchive&>(ar);
            data = inAr.read_buffer(SerializedElementSize * Num);
        }
    };

    struct SATISFACTORYSAVE_API FStaticMeshVertexBuffer {
        uint32_t NumTexCoords = 0;
        uint32_t NumVertices = 0;
        bool bUseFullPrecisionUVs = false;
        bool bUseHighPrecisionTangentBasis = false;

        ResourceArray TangentsData;
        ResourceArray TexcoordData;

        void serialize(Archive& ar) {
            FStripDataFlags dataFlags;
            ar << dataFlags;
            dataFlags.validateOnlyEditorDataIsStripped();

            ar << NumTexCoords;
            ar << NumVertices;
            ar << bUseFullPrecisionUVs;
            ar << bUseHighPrecisionTangentBasis;

            ar << TangentsData;
            ar << TexcoordData;
        }
    };

    struct SATISFACTORYSAVE_API FPositionVertexBuffer {
        uint32_t Stride = 0;
        uint32_t NumVertices = 0;
        ResourceArray VertexData;

        void serialize(Archive& ar) {
            ar << Stride;
            ar << NumVertices;

            ar << VertexData;
        }
    };

    struct SATISFACTORYSAVE_API FColorVertexBuffer {
        uint32_t Stride = 0;
        uint32_t NumVertices = 0;

        ResourceArray VertexData;

        void serialize(Archive& ar) {
            FStripDataFlags dataFlags;
            ar << dataFlags;
            dataFlags.validateOnlyEditorDataIsStripped();

            ar << Stride;
            ar << NumVertices;

            if (NumVertices > 0) {
                ar << VertexData;
            }
        }
    };

    struct SATISFACTORYSAVE_API FStaticMeshVertexBuffers {
        FStaticMeshVertexBuffer StaticMeshVertexBuffer;
        FPositionVertexBuffer PositionVertexBuffer;
        FColorVertexBuffer ColorVertexBuffer;
    };

    struct SATISFACTORYSAVE_API FRawStaticIndexBuffer {
        ResourceArray IndexStorage;
        bool b32Bit = false;
        bool bShouldExpandTo32Bit = false;

        void serialize(Archive& ar) {
            ar << b32Bit;
            ar << IndexStorage;
            ar << bShouldExpandTo32Bit;
        }
    };

    struct SATISFACTORYSAVE_API FAdditionalStaticMeshIndexBuffers {
        FRawStaticIndexBuffer ReversedIndexBuffer;
        FRawStaticIndexBuffer ReversedDepthOnlyIndexBuffer;
        FRawStaticIndexBuffer WireframeIndexBuffer;
    };

    struct SATISFACTORYSAVE_API FWeightedRandomSampler {
        std::vector<float> Prob;    // TArray<float>
        std::vector<int32_t> Alias; // TArray<int32>
        float TotalWeight = 0.0f;

        void serialize(Archive& ar) {
            ar << Prob;
            ar << Alias;
            ar << TotalWeight;
        }
    };

    struct SATISFACTORYSAVE_API FStaticMeshBuffersSize {
        uint32_t SerializedBuffersSize = 0;
        uint32_t DepthOnlyIBSize = 0;
        uint32_t ReversedIBsSize = 0;

        void serialize(Archive& ar) {
            ar << SerializedBuffersSize;
            ar << DepthOnlyIBSize;
            ar << ReversedIBsSize;
        }
    };

    struct SATISFACTORYSAVE_API FStaticMeshLODResources {
        enum EClassDataStripFlag : uint8_t {
            CDSF_AdjacencyData = 1,
            CDSF_MinLodData = 2,
            CDSF_ReversedIndexBuffer = 4,
            CDSF_RayTracingResources = 8,
        };

        std::vector<FStaticMeshSection> Sections;
        float MaxDeviation = 0.0f;
        FStaticMeshVertexBuffers VertexBuffers;
        FRawStaticIndexBuffer IndexBuffer;
        FRawStaticIndexBuffer DepthOnlyIndexBuffer;
        FAdditionalStaticMeshIndexBuffers AdditionalIndexBuffers;
        FWeightedRandomSampler AreaWeightedSampler; // FStaticMeshAreaWeightedSectionSampler
        std::vector<FWeightedRandomSampler>
            AreaWeightedSectionSamplers; // FStaticMeshSectionAreaWeightedTriangleSamplerArray

        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Engine/Private/StaticMesh.cpp#L649
        void serialize(Archive& ar) {
            FStripDataFlags dataFlags;
            ar << dataFlags;

            ar << Sections;
            ar << MaxDeviation;

            bool bIsLODCookedOut = false;
            ar << bIsLODCookedOut;
            if (bIsLODCookedOut) {
                throw std::runtime_error("bIsLODCookedOut == true not implemented!");
            }

            bool bInlined = false;
            ar << bInlined;

            FStaticMeshBuffersSize TmpBuffersSize;

            if (bInlined) {
                serializeBuffers(ar);
                ar << TmpBuffersSize;
            } else {
                throw std::runtime_error("bInlined == false not implemented!");
            }
        }

        // FStaticMeshLODResources::SerializeBuffers
        void serializeBuffers(Archive& ar) {
            FStripDataFlags StripFlags;
            ar << StripFlags;

            ar << VertexBuffers.PositionVertexBuffer;
            ar << VertexBuffers.StaticMeshVertexBuffer;
            ar << VertexBuffers.ColorVertexBuffer;

            ar << IndexBuffer;

            const bool bSerializeReversedIndexBuffer = !StripFlags.IsClassDataStripped(CDSF_ReversedIndexBuffer);
            const bool bSerializeWireframeIndexBuffer = !StripFlags.IsEditorDataStripped();
            const bool bSerializeRayTracingGeometry = !StripFlags.IsClassDataStripped(CDSF_RayTracingResources);

            if (bSerializeReversedIndexBuffer) {
                ar << AdditionalIndexBuffers.ReversedIndexBuffer;
            }

            ar << DepthOnlyIndexBuffer;

            if (bSerializeReversedIndexBuffer) {
                ar << AdditionalIndexBuffers.ReversedDepthOnlyIndexBuffer;
            }

            if (bSerializeWireframeIndexBuffer) {
                ar << AdditionalIndexBuffers.WireframeIndexBuffer;
            }

            if (bSerializeRayTracingGeometry) {
                throw std::runtime_error("RayTracingGeometry not implemented!");
            }

            AreaWeightedSectionSamplers.resize(Sections.size());
            for (auto& Sampler : AreaWeightedSectionSamplers) {
                ar << Sampler;
            }
            ar << AreaWeightedSampler;
        }
    };

    struct SATISFACTORYSAVE_API FStaticMeshRenderData {
        std::vector<FStaticMeshLODResources> LODResources;

        // TODO ...

        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Engine/Private/StaticMesh.cpp#L1459
        void serialize(Archive& ar) {
            ar << LODResources;

            // TODO ...
        }
    };

    // FStaticMesh
    class SATISFACTORYSAVE_API StaticMesh {
    public:
        StaticMesh() = default;

        void serialize(Archive& ar);

        [[nodiscard]] const FStaticMeshRenderData& renderData() const {
            return RenderData;
        }

    protected:
        PropertyList properties_;
        std::optional<FGuid> guid_;
        FStaticMeshRenderData RenderData;
    };
} // namespace SatisfactorySave
