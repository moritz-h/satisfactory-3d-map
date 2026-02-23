#pragma once

#include <optional>

#include "../../../../IO/Archive/Archive.h"
#include "../../../Properties/Base/PropertyList.h"
#include "../../Core/Misc/Guid.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API UObject {
    public:
        UObject() = default;
        virtual ~UObject() = default;

        PropertyList Properties;
        std::optional<FGuid> Guid;

        virtual void serialize(Archive& ar) {
            if (ar.getSaveVersion() >= 53) {
                /*EClassSerializationControlExtension*/ uint8_t SerializationControl = 0;
                ar << SerializationControl;
                if (SerializationControl != 0) {
                    throw std::runtime_error("SerializationControl != 0 not implemented!");
                }
            }
            ar << Properties;
            ar << Guid;
        }
    };

} // namespace SatisfactorySave
