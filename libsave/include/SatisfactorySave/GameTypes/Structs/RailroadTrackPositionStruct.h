#pragma once

#include "../UE/Satisfactory/ObjectReference.h"
#include "Struct.h"

namespace SatisfactorySave {

    class RailroadTrackPositionStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] const ObjectReference& ref() const {
            return ref_;
        }

        [[nodiscard]] float offset() const {
            return offset_;
        }

        [[nodiscard]] float forward() const {
            return forward_;
        }

    protected:
        ObjectReference ref_;
        float offset_ = 0.0f;
        float forward_ = 0.0f;
    };
} // namespace SatisfactorySave
