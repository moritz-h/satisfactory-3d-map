#pragma once

#include <vector>

#include "Set.h"

namespace Satisfactory3DMap {

    class UInt32Set : public Set {
    public:
        using Set::Set;

        void serialize(Archive& ar) override;

        void accept(SetVisitor& v) override;

        [[nodiscard]] const std::vector<uint32_t>& set() const {
            return set_;
        }

    protected:
        std::vector<uint32_t> set_;
    };
} // namespace Satisfactory3DMap
