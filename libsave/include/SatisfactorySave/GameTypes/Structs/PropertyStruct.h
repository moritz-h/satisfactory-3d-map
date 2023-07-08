#pragma once

#include <memory>
#include <vector>

#include "../Properties/Properties.h"
#include "Struct.h"

namespace SatisfactorySave {

    class PropertyStruct : public Struct {
    public:
        explicit PropertyStruct(FName name);

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] const Properties& properties() const {
            return properties_;
        }

    protected:
        Properties properties_;
    };
} // namespace SatisfactorySave
