#ifndef SATISFACTORY3DMAP_MAPPROPERTY_H
#define SATISFACTORY3DMAP_MAPPROPERTY_H

#include <vector>

#include "../MapTypes/MapTypeList.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class MapProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const std::string& keyType() const {
            return tag_.InnerType;
        }

        [[nodiscard]] const std::string& valueType() const {
            return tag_.ValueType;
        }

        [[nodiscard]] const std::unique_ptr<MapTypeList>& keys() const {
            return keys_;
        }

        [[nodiscard]] const std::unique_ptr<MapTypeList>& values() const {
            return values_;
        }

    protected:
        std::unique_ptr<MapTypeList> keys_;
        std::unique_ptr<MapTypeList> values_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MAPPROPERTY_H
