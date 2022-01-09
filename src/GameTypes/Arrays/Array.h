#ifndef SATISFACTORY3DMAP_ARRAY_H
#define SATISFACTORY3DMAP_ARRAY_H

#include <cstdint>
#include <memory>
#include <string>

#include "IO/Archive/Archive.h"

namespace Satisfactory3DMap {

    class ArrayVisitor;

    class Array {
    public:
        static std::unique_ptr<Array> create(const std::string& array_type, Archive& ar);

        explicit Array(std::string array_type) : array_type_(std::move(array_type)) {}
        virtual ~Array() = default;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(ArrayVisitor& v) = 0;

        [[nodiscard]] const std::string& type() const {
            return array_type_;
        }

    protected:
        std::string array_type_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ARRAY_H
