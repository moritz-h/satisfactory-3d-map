#pragma once

#include <memory>
#include <vector>

#include "IO/Archive/Archive.h"
#include "Property.h"

namespace Satisfactory3DMap {

    class Properties {
    public:
        explicit Properties() = default;

        void serialize(Archive& ar);

        void setParentClass(const std::string& parentClassName) {
            parentClassName_ = parentClassName;
        }

        [[nodiscard]] const std::vector<std::unique_ptr<Property>>& properties() const {
            return properties_;
        }

        auto begin() {
            return properties_.begin();
        }

        auto end() {
            return properties_.end();
        }

        [[nodiscard]] auto cbegin() const {
            return properties_.cbegin();
        }

        [[nodiscard]] auto cend() const {
            return properties_.cend();
        }

        [[nodiscard]] auto begin() const {
            return properties_.begin();
        }

        [[nodiscard]] auto end() const {
            return properties_.end();
        }

        [[nodiscard]] auto empty() const {
            return properties_.empty();
        }

        [[nodiscard]] auto size() const {
            return properties_.size();
        }

        [[nodiscard]] const Property& at(std::size_t n) const {
            return *properties_.at(n);
        }

        template<typename T>
        inline const T& get(const std::string& name) const {
            for (const auto& p : properties_) {
                if (p->tag().Name == name) {
                    const T* property = dynamic_cast<const T*>(p.get());
                    if (property != nullptr) {
                        return *property;
                    }
                    throw std::runtime_error("Property type invalid: " + name);
                }
            }
            throw std::runtime_error("Property name invalid: " + name);
        }

    protected:
        std::vector<std::unique_ptr<Property>> properties_;
        std::string parentClassName_;
    };
} // namespace Satisfactory3DMap
