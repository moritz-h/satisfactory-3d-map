#pragma once

#include <vector>

#include "../MapTypes/MapTypeList.h"
#include "Base/Property.h"

namespace SatisfactorySave {

    class MapProperty : public Property {
    public:
        static constexpr std::string_view TypeName = "MapProperty";

        MapProperty(PropertyTag tag, std::string parentClassName);

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const FName& keyType() const {
            return Tag.InnerType;
        }

        [[nodiscard]] const FName& valueType() const {
            return Tag.ValueType;
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
} // namespace SatisfactorySave
