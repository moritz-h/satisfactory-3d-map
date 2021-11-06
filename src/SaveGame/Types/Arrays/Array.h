#ifndef SATISFACTORY3DMAP_ARRAY_H
#define SATISFACTORY3DMAP_ARRAY_H

#include <cstdint>
#include <memory>
#include <string>

namespace Satisfactory3DMap {

    class ArrayVisitor;

    class Array {
    public:
        static std::unique_ptr<Array> parse(const std::string& array_type, int32_t count, std::istream& stream);

        explicit Array(std::string array_type) : array_type_(std::move(array_type)) {}
        virtual ~Array() = default;

        virtual void serialize(std::ostream& stream) const = 0;

        virtual void accept(ArrayVisitor& v) = 0;

        const std::string& type() const {
            return array_type_;
        }

    protected:
        std::string array_type_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ARRAY_H
