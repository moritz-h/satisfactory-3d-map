#pragma once

#include <cstdint>

#include "../../../../IO/Archive/Archive.h"
#include "../../Core/Containers/DynamicRHIResourceArray.h"
#include "../EngineUtils.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FStaticMeshVertexBuffer {
        uint32_t NumTexCoords = 0;
        uint32_t NumVertices = 0;
        bool bUseFullPrecisionUVs = false;
        bool bUseHighPrecisionTangentBasis = false;

        /*FStaticMeshVertexDataInterface*/ TResourceArray TangentsData;
        /*FStaticMeshVertexDataInterface*/ TResourceArray TexcoordData;

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
} // namespace SatisfactorySave
