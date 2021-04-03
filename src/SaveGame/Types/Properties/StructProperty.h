#ifndef SATISFACTORY3DMAP_STRUCTPROPERTY_H
#define SATISFACTORY3DMAP_STRUCTPROPERTY_H

#include <vector>

#include "Property.h"

namespace Satisfactory3DMap {

    class StructProperty : public Property {
    public:
        StructProperty(std::string property_name, std::string property_type, std::istream& stream);

        const std::string& structName() const {
            return struct_name_;
        }

        int32_t unk1() const {
            return unk1_;
        }

        int32_t unk2() const {
            return unk2_;
        }

        int32_t unk3() const {
            return unk3_;
        }

        int32_t unk4() const {
            return unk4_;
        }

        const std::vector<char>& buf() const {
            return buf_;
        }

    protected:
        std::string struct_name_;
        int32_t unk1_;
        int32_t unk2_;
        int32_t unk3_;
        int32_t unk4_;

        std::vector<char> buf_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRUCTPROPERTY_H
