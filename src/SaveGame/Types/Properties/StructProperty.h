#ifndef SATISFACTORYSAVEGAME_STRUCTPROPERTY_H
#define SATISFACTORYSAVEGAME_STRUCTPROPERTY_H

#include "Property.h"

namespace SatisfactorySaveGame {

    class StructProperty : public Property {
    public:
        StructProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
        std::string struct_name_;
        int32_t unk1_;
        int32_t unk2_;
        int32_t unk3_;
        int32_t unk4_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_STRUCTPROPERTY_H
