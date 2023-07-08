#pragma once

#include <string>
#include <vector>

#include "../Structs/Struct.h"
#include "Set.h"

namespace SatisfactorySave {

    class StructSet : public Set {
    public:
        StructSet(FName type, const FName& name, const std::string& parentClassName);

        void serialize(Archive& ar) override;

        void accept(SetVisitor& v) override;

        [[nodiscard]] const FName& structName() const {
            return struct_name_;
        }

        [[nodiscard]] const std::vector<std::unique_ptr<Struct>>& set() const {
            return set_;
        }

    protected:
        FName struct_name_;
        std::vector<std::unique_ptr<Struct>> set_;
    };
} // namespace SatisfactorySave
