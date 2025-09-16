#pragma once

#include <cstdint>

#include "../../../../IO/Archive/Archive.h"
#include "../../Core/Containers/DynamicRHIResourceArray.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FPositionVertexBuffer {
        uint32_t Stride = 0;
        uint32_t NumVertices = 0;
        /*FPositionVertexData*/ TResourceArray VertexData;

        void serialize(Archive& ar) {
            ar << Stride;
            ar << NumVertices;

            ar << VertexData;
        }
    };
} // namespace SatisfactorySave
