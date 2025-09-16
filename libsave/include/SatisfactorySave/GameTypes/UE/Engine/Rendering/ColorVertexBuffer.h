#pragma once

#include <cstdint>

#include "../../../../IO/Archive/Archive.h"
#include "../../Core/Containers/DynamicRHIResourceArray.h"
#include "../EngineUtils.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FColorVertexBuffer {
        uint32_t Stride = 0;
        uint32_t NumVertices = 0;

        /*FColorVertexData*/ TResourceArray VertexData;

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
} // namespace SatisfactorySave
