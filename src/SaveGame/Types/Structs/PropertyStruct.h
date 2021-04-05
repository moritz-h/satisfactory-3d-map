#ifndef SATISFACTORY3DMAP_PROPERTYSTRUCT_H
#define SATISFACTORY3DMAP_PROPERTYSTRUCT_H

#include "../Properties/Property.h"
#include "Struct.h"
#include "Utils/StreamUtils.h"

namespace Satisfactory3DMap {

    class PropertyStruct : public Struct {
    public:
        PropertyStruct(std::string struct_name, std::istream& stream) : Struct(std::move(struct_name)) {
            bool done = false;
            do {
                auto property = Property::parse(stream);
                if (property == nullptr) {
                    done = true;
                } else {
                    properties_.emplace_back(std::move(property));
                }
            } while (!done);
        }

    protected:
        std::vector<std::unique_ptr<Property>> properties_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PROPERTYSTRUCT_H
