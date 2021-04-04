#ifndef SATISFACTORY3DMAP_STRUCTPROPERTY_H
#define SATISFACTORY3DMAP_STRUCTPROPERTY_H

#include <vector>

#include "../Guid.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class StructProperty : public Property {
    public:
        StructProperty(std::string property_name, std::string property_type, std::istream& stream);

        const std::string& structName() const {
            return struct_name_;
        }

        std::string guid() const {
            return guid_.toString();
        }

        const std::vector<char>& buf() const {
            return buf_;
        }

    protected:
        std::string struct_name_;
        Guid guid_;

        std::vector<char> buf_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRUCTPROPERTY_H
