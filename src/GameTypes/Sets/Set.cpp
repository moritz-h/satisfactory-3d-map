#include "Set.h"

#include <stdexcept>

#include "StructSet.h"

std::unique_ptr<Satisfactory3DMap::Set> Satisfactory3DMap::Set::create(const FName& set_type, const FName& name,
    const std::string& parentClassName, Archive& ar) {
    std::unique_ptr<Set> set;

    if (set_type == "StructProperty") {
        set = std::make_unique<StructSet>(set_type, name, parentClassName);
    } else {
        throw std::runtime_error("Set type \"" + set_type + "\" not implemented!");
    }

    ar << *set;

    return set;
}
