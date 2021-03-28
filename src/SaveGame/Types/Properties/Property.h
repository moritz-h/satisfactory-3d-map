#ifndef SATISFACTORY3DMAP_PROPERTY_H
#define SATISFACTORY3DMAP_PROPERTY_H

#include <cstdint>
#include <string>

namespace Satisfactory3DMap {

    class Property {
    public:
        Property(std::string property_name, std::string property_type, std::istream& stream);

        const std::string& name() const {
            return property_name_;
        }

        const std::string& type() const {
            return property_type_;
        }

        int32_t size() const {
            return size_;
        }

        int32_t index() const {
            return index_;
        }

    protected:
        std::string property_name_;
        std::string property_type_;
        int32_t size_;
        int32_t index_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PROPERTY_H
