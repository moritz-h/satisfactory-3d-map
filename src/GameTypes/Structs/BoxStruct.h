#pragma once

#include <glm/vec3.hpp>

#include "Struct.h"

namespace Satisfactory3DMap {

    class BoxStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] const glm::vec3& min() const {
            return min_;
        }

        [[nodiscard]] const glm::vec3& max() const {
            return max_;
        }

        [[nodiscard]] uint8_t isValid() const {
            return is_valid_;
        }

    protected:
        glm::vec3 min_ = glm::vec3(0.0f);
        glm::vec3 max_ = glm::vec3(0.0f);
        uint8_t is_valid_ = 0;
    };
} // namespace Satisfactory3DMap
