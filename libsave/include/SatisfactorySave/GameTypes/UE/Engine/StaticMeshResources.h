#pragma once

#include <cstdint>
#include <vector>

#include "../../../IO/Archive/Archive.h"
#include "../Core/Math/BoxSphereBounds.h"
#include "DistanceFieldAtlas.h"
#include "EngineUtils.h"
#include "MeshCardBuild.h"
#include "PerPlatformProperties.h"
#include "RawIndexBuffer.h"
#include "Rendering/ColorVertexBuffer.h"
#include "Rendering/NaniteResources.h"
#include "Rendering/PositionVertexBuffer.h"
#include "Rendering/StaticMeshVertexBuffer.h"
#include "WeightedRandomSampler.h"
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

    struct SATISFACTORYSAVE_API FStaticMeshVertexBuffers {
        FStaticMeshVertexBuffer StaticMeshVertexBuffer;
        FPositionVertexBuffer PositionVertexBuffer;
        FColorVertexBuffer ColorVertexBuffer;
    };

    struct SATISFACTORYSAVE_API FAdditionalStaticMeshIndexBuffers {
        FRawStaticIndexBuffer ReversedIndexBuffer;
        FRawStaticIndexBuffer ReversedDepthOnlyIndexBuffer;
        FRawStaticIndexBuffer WireframeIndexBuffer;
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

        FCardRepresentationData CardRepresentationData;
        FDistanceFieldVolumeData DistanceFieldData;

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
        uint8_t NumInlinedLODs = 0;
        FResources NaniteResources;
        FBoxSphereBounds Bounds;
        bool bLODsShareStaticLighting = false;
        std::array<FPerPlatformFloat, /*MAX_STATIC_MESH_LODS*/ 8> ScreenSize;

        // https://github.com/EpicGames/UnrealEngine/blob/5.3.2-release/Engine/Source/Runtime/Engine/Private/StaticMesh.cpp#L1683
        void serialize(Archive& ar) {
            ar << LODResources;
            ar << NumInlinedLODs;
            ar << NaniteResources;

            // FStaticMeshRenderData::SerializeInlineDataRepresentations
            FStripDataFlags StripFlags;
            ar << StripFlags;
            StripFlags.validateOnlyEditorDataIsStripped();

            for (auto& LOD : LODResources) {
                bool bValid = true;
                ar << bValid;

                if (bValid) {
                    ar << LOD.CardRepresentationData;
                }
            }
            // End FStaticMeshRenderData::SerializeInlineDataRepresentations

            ar << StripFlags;
            StripFlags.validateOnlyEditorDataIsStripped();

            for (auto& LOD : LODResources) {
                bool bValid = true;
                ar << bValid;

                if (bValid) {
                    ar << LOD.DistanceFieldData;
                }
            }

            ar << Bounds;
            ar << bLODsShareStaticLighting;

            for (auto& screenSizeEntry : ScreenSize) {
                ar << screenSizeEntry;
            }
        }
    };
} // namespace SatisfactorySave
