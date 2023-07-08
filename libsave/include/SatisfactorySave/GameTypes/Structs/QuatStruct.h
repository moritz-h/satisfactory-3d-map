#pragma once

#include "Struct.h"

namespace SatisfactorySave {

    class QuatStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] double x() const {
            return x_;
        }

        [[nodiscard]] double y() const {
            return y_;
        }

        [[nodiscard]] double z() const {
            return z_;
        }

        [[nodiscard]] double w() const {
            return w_;
        }

    protected:
        double x_ = 0.0f;
        double y_ = 0.0f;
        double z_ = 0.0f;
        double w_ = 0.0f;
    };
} // namespace SatisfactorySave
