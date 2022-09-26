#pragma once

#include <vector>

#include "../MapTypes/MapTypeList.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class MapProperty : public Property {
    public:
        MapProperty(PropertyTag tag, std::string parentClassName);

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const FName& keyType() const {
            return tag_.InnerType;
        }

        [[nodiscard]] const FName& valueType() const {
            return tag_.ValueType;
        }

        [[nodiscard]] const std::unique_ptr<MapTypeList>& keys() const {
            return keys_;
        }

        [[nodiscard]] const std::unique_ptr<MapTypeList>& values() const {
            return values_;
        }

    protected:
        std::string parentClassName_;
        std::unique_ptr<MapTypeList> keys_;
        std::unique_ptr<MapTypeList> values_;
    };
} // namespace Satisfactory3DMap
