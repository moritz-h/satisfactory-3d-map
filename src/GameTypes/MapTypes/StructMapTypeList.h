#pragma once

#include <vector>

#include "../Structs/Struct.h"
#include "MapTypeList.h"

namespace Satisfactory3DMap {

    class StructMapTypeList : public MapTypeList {
    public:
        StructMapTypeList(FName type, const FName& name, const std::string& parentClassName);

        void accept(MapTypeListVisitor& v) override;

        void serializeEntry(Archive& ar, std::size_t i) override;

        [[nodiscard]] std::size_t listSize() const override {
            return list_.size();
        }

        [[nodiscard]] const std::vector<std::unique_ptr<Struct>>& list() const {
            return list_;
        }

    protected:
        FName struct_name_;
        std::vector<std::unique_ptr<Struct>> list_;
    };
} // namespace Satisfactory3DMap
