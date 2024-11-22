#pragma once

#include <memory>
#include <vector>

#include "../../../IO/Archive/Archive.h"
#include "Property.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API PropertyList : public std::vector<std::shared_ptr<Property>> {
    public:
        PropertyList() = default;
        ~PropertyList() = default;
        PropertyList(const PropertyList& other);
        PropertyList& operator=(const PropertyList& other);
        PropertyList(PropertyList&&) = default;
        PropertyList& operator=(PropertyList&&) = default;

        void serialize(Archive& ar);

        [[nodiscard]] inline const std::shared_ptr<Property>& getPtr(const std::string& name) const {
            for (const auto& p : *this) {
                if (p->Name() == name) {
                    return p;
                }
            }
            throw std::runtime_error("Property name invalid: " + name);
        }

        template<typename T>
        inline T& get(const std::string& name) const {
            T* property = dynamic_cast<T*>(getPtr(name).get());
            if (property != nullptr) {
                return *property;
            }
            throw std::runtime_error("Property type invalid: " + name);
        }
    };
} // namespace SatisfactorySave
