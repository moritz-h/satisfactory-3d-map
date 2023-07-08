#pragma once

#include "Struct.h"

namespace SatisfactorySave {

    class ColorStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] int8_t b() const {
            return b_;
        }

        [[nodiscard]] int8_t g() const {
            return g_;
        }

        [[nodiscard]] int8_t r() const {
            return r_;
        }

        [[nodiscard]] int8_t a() const {
            return a_;
        }

    protected:
        int8_t b_ = 0;
        int8_t g_ = 0;
        int8_t r_ = 0;
        int8_t a_ = 0;
    };
} // namespace SatisfactorySave
