#pragma once

#include <memory>
#include <vector>

#include "../../../IO/Archive/Archive.h"
#include "Property.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API PropertyList : public std::vector<std::unique_ptr<Property>> {
    public:
        PropertyList() = default;
        ~PropertyList() = default;
        PropertyList(const PropertyList& other);
        PropertyList& operator=(const PropertyList& other);
        PropertyList(PropertyList&&) = default;
        PropertyList& operator=(PropertyList&&) = default;

        void serialize(Archive& ar);

        template<typename T>
        inline T& get(const std::string& name) const {
            for (const auto& p : *this) {
                if (p->name() == name) {
                    T* property = dynamic_cast<T*>(p.get());
                    if (property != nullptr) {
                        return *property;
                    }
                    throw std::runtime_error("Property type invalid: " + name);
                }
            }
            throw std::runtime_error("Property name invalid: " + name);
        }
    };
} // namespace SatisfactorySave
