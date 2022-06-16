#pragma once

#include <string>
#include <vector>

#include "../Structs/Struct.h"
#include "Set.h"

namespace Satisfactory3DMap {

    class StructSet : public Set {
    public:
        StructSet(FName type, FName name, std::string parentClassName);

        void serialize(Archive& ar) override;

        void accept(SetVisitor& v) override;

        [[nodiscard]] const FName& name() const {
            return name_;
        }

        [[nodiscard]] const FName& structName() const {
            return struct_name_;
        }

        [[nodiscard]] const std::vector<std::unique_ptr<Struct>>& set() const {
            return set_;
        }

    protected:
        FName name_;
        std::string parentClassName_;
        FName struct_name_;
        std::vector<std::unique_ptr<Struct>> set_;
    };
} // namespace Satisfactory3DMap
