#ifndef SATISFACTORY3DMAP_PROPERTIES_H
#define SATISFACTORY3DMAP_PROPERTIES_H

#include <memory>
#include <vector>

#include "IO/Archive/Archive.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class Properties {
    public:
        explicit Properties() = default;
        ~Properties() = default;

        void serialize(Archive& ar);

        [[nodiscard]] const std::vector<std::unique_ptr<Property>>& properties() const {
            return properties_;
        }

        auto begin() {
            return properties_.begin();
        }

        auto end() {
            return properties_.end();
        }

        auto cbegin() const {
            return properties_.cbegin();
        }

        auto cend() const {
            return properties_.cend();
        }

        auto begin() const {
            return properties_.begin();
        }

        auto end() const {
            return properties_.end();
        }

        auto empty() const {
            return properties_.empty();
        }

        auto size() const {
            return properties_.size();
        }

        const Property& at(std::size_t n) const {
            return *properties_.at(n);
        }

    protected:
        std::vector<std::unique_ptr<Property>> properties_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PROPERTIES_H
