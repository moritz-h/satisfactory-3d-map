#ifndef SATISFACTORY3DMAP_STRUCTPROPERTY_H
#define SATISFACTORY3DMAP_STRUCTPROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

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
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRUCTPROPERTY_H
