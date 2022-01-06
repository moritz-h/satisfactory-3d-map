#ifndef SATISFACTORY3DMAP_MAPPROPERTY_H
#define SATISFACTORY3DMAP_MAPPROPERTY_H

#include <vector>

#include "../MapTypes/MapTypeList.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class MapProperty : public Property {
    public:
        MapProperty(PropertyTag tag, std::istream& stream);

        void serialize(std::ostream& stream) const override;

        void accept(PropertyVisitor& v) override;

        const std::string& keyType() const {
            return tag_.InnerType;
        }

        const std::string& valueType() const {
            return tag_.ValueType;
        }

        const std::unique_ptr<MapTypeList>& keys() const {
            return keys_;
        }

        const std::unique_ptr<MapTypeList>& values() const {
            return values_;
        }

    protected:
        std::unique_ptr<MapTypeList> keys_;
        std::unique_ptr<MapTypeList> values_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MAPPROPERTY_H
