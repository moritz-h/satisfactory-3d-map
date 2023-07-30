#pragma once

#include <vector>

#include "../Structs/Base/Struct.h"
#include "Base/MapTypeList.h"

namespace SatisfactorySave {

    class StructMapTypeList : public MapTypeList {
    public:
        static constexpr std::string_view TypeName = "StructProperty";

        StructMapTypeList(const FName& name, const std::string& parentClassName, bool isKey = false);

        void serializeEntry(Archive& ar, std::size_t i) override;

        void accept(MapTypeListVisitor& v) override;

        [[nodiscard]] std::size_t listSize() const override {
            return List.size();
        }

        std::vector<std::unique_ptr<Struct>> List;

    protected:
        FName struct_name_;
    };
} // namespace SatisfactorySave
