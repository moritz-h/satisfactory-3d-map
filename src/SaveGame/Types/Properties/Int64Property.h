#ifndef SATISFACTORY3DMAP_INT64PROPERTY_H
#define SATISFACTORY3DMAP_INT64PROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

    class Int64Property : public Property {
    public:
        Int64Property(std::string property_name, std::string property_type, std::istream& stream);

        int64_t value() const {
            return value_;
        }

    protected:
        int64_t value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_INT64PROPERTY_H
