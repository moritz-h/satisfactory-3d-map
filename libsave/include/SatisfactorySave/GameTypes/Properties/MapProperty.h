#pragma once

#include <vector>

#include "../MapTypes/Base/MapTypeList.h"
#include "Base/Property.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API MapProperty : public Property {
    public:
        static constexpr std::string_view TypeName = "MapProperty";

        MapProperty(PropertyTag tag);

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] inline FName& keyType() {
            return tag_.InnerType;
        }

        [[nodiscard]] inline FName& valueType() {
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
} // namespace SatisfactorySave
