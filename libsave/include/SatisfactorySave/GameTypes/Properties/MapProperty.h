#pragma once

#include <vector>

#include "../MapTypes/Base/MapTypeList.h"
#include "Base/Property.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API MapProperty final : public Property {
    public:
        static constexpr std::string_view TypeName = "MapProperty";

        MapProperty() : Property(FName(std::string(TypeName))) {}
        explicit MapProperty(FName name) : Property(FName(std::string(TypeName)), std::move(name)) {}
        MapProperty(FName name, std::unique_ptr<MapTypeList> keys, std::unique_ptr<MapTypeList> values);
        explicit MapProperty(PropertyTag tag) : Property(std::move(tag)) {}
        MapProperty(const MapProperty& other);
        MapProperty& operator=(const MapProperty& other);
        MapProperty(MapProperty&&) = default;
        MapProperty& operator=(MapProperty&&) = default;

        [[nodiscard]] std::unique_ptr<Property> clone() const override;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] inline FName& keyType() {
            return tag_.InnerType;
        }

        [[nodiscard]] inline FName& valueType() {
            return tag_.ValueType;
        }

        std::unique_ptr<MapTypeList> Keys;
        std::unique_ptr<MapTypeList> Values;
    };
} // namespace SatisfactorySave
