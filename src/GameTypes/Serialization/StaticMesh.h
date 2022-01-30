#ifndef SATISFACTORY3DMAP_STATICMESH_H
#define SATISFACTORY3DMAP_STATICMESH_H

#include "../Guid.h"
#include "../Properties/Properties.h"
#include "IO/Archive/Archive.h"
#include "StripDataFlags.h"

namespace Satisfactory3DMap {

    struct FStaticMeshSection {
        int32_t MaterialIndex = 0;
        uint32_t FirstIndex = 0;
        uint32_t NumTriangles = 0;
        uint32_t MinVertexIndex = 0;
        uint32_t MaxVertexIndex = 0;
        bool bEnableCollision = false;
        bool bCastShadow = false;
        bool bForceOpaque = false;
        bool bVisibleInRayTracing = false;

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
        }
    };

    struct FVertexResourceArray {
        std::vector<char> data;

        void serialize(Archive& ar) {
            // BulkSerialize
            int32_t SerializedElementSize;
            ar << SerializedElementSize;

            int32_t Num;
            ar << Num;

            auto& inAr = dynamic_cast<IStreamArchive&>(ar);
            data = inAr.read_vector<char>(SerializedElementSize * Num);
        }
    };

    struct FStaticMeshVertexBuffer {
        uint32_t NumTexCoords = 0;
        uint32_t NumVertices = 0;
        bool bUseFullPrecisionUVs = false;
        bool bUseHighPrecisionTangentBasis = false;

        FVertexResourceArray TangentsData;
        FVertexResourceArray TexcoordData;

        void serialize(Archive& ar) {
            FStripDataFlags dataFlags;
            ar << dataFlags;

            ar << NumTexCoords;
            ar << NumVertices;
            ar << bUseFullPrecisionUVs;
            ar << bUseHighPrecisionTangentBasis;

            ar << TangentsData;
            ar << TexcoordData;
        }
    };

    struct FPositionVertexBuffer {
        uint32_t Stride = 0;
        uint32_t NumVertices = 0;
        FVertexResourceArray VertexData;

        void serialize(Archive& ar) {
            ar << Stride;
            ar << NumVertices;

            ar << VertexData;
        }
    };

    struct FColorVertexBuffer {
        uint32_t Stride = 0;
        uint32_t NumVertices = 0;

        FVertexResourceArray VertexData;

        void serialize(Archive& ar) {
            FStripDataFlags dataFlags;
            ar << dataFlags;

            ar << Stride;
            ar << NumVertices;

            ar << VertexData;
        }
    };

    struct FStaticMeshVertexBuffers {
        FStaticMeshVertexBuffer StaticMeshVertexBuffer;
        FPositionVertexBuffer PositionVertexBuffer;
        FColorVertexBuffer ColorVertexBuffer;
    };

    struct FStaticMeshLODResources {
        std::vector<FStaticMeshSection> Sections;
        float MaxDeviation = 0.0f;
        FStaticMeshVertexBuffers VertexBuffers;

        // TODO ...

        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Engine/Private/StaticMesh.cpp#L649
        void serialize(Archive& ar) {
            FStripDataFlags dataFlags;
            ar << dataFlags;

            int32_t SectionsNum = 0;
            ar << SectionsNum;
            Sections.resize(SectionsNum);
            for (auto& sec : Sections) {
                ar << sec;
            }

            ar << MaxDeviation;

            bool bIsLODCookedOut = false;
            ar << bIsLODCookedOut;

            bool bInlined = false;
            ar << bInlined;

            if (!bInlined) {
                throw std::runtime_error("bInlined == false not implemented!");
            }

            // FStaticMeshLODResources::SerializeBuffers

            ar << dataFlags;

            ar << VertexBuffers.PositionVertexBuffer;
            ar << VertexBuffers.StaticMeshVertexBuffer;
            ar << VertexBuffers.ColorVertexBuffer;

            // TODO ...
        }
    };

    struct FStaticMeshRenderData {
        std::vector<FStaticMeshLODResources> LODResources;

        // TODO ...

        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Engine/Private/StaticMesh.cpp#L1459
        void serialize(Archive& ar) {
            int32_t Num = 0;
            ar << Num;
            LODResources.resize(Num);
            for (auto& res : LODResources) {
                ar << res;
            }

            // TODO ...
        }
    };

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
