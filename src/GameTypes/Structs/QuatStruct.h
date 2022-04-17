#pragma once

#include "Struct.h"

namespace Satisfactory3DMap {

    class QuatStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] float x() const {
            return x_;
        }

        [[nodiscard]] float y() const {
            return y_;
        }

        [[nodiscard]] float z() const {
            return z_;
        }

        [[nodiscard]] float w() const {
            return w_;
        }

    protected:
        float x_ = 0.0f;
        float y_ = 0.0f;
        float z_ = 0.0f;
        float w_ = 0.0f;
    };
} // namespace Satisfactory3DMap
