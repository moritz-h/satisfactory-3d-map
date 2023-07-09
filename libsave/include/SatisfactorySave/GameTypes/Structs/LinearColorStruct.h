#pragma once

#include "Base/Struct.h"

namespace SatisfactorySave {

    class LinearColorStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] float r() const {
            return r_;
        }

        [[nodiscard]] float g() const {
            return g_;
        }

        [[nodiscard]] float b() const {
            return b_;
        }

        [[nodiscard]] float a() const {
            return a_;
        }

    protected:
        float r_ = 0.0f;
        float g_ = 0.0f;
        float b_ = 0.0f;
        float a_ = 0.0f;
    };
} // namespace SatisfactorySave
