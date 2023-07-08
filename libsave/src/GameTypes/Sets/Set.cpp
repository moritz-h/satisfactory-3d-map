#include "GameTypes/Sets/Set.h"

#include <stdexcept>

#include "GameTypes/Sets/StructSet.h"
#include "GameTypes/Sets/UInt32Set.h"

std::unique_ptr<Satisfactory3DMap::Set> Satisfactory3DMap::Set::create(const FName& set_type, const FName& name,
    const std::string& parentClassName, Archive& ar) {
    std::unique_ptr<Set> set;

    if (set_type == "StructProperty") {
        set = std::make_unique<StructSet>(set_type, name, parentClassName);
    } else if (set_type == "UInt32Property") {
        set = std::make_unique<UInt32Set>(set_type);
    } else {
        throw std::runtime_error("Set type \"" + set_type + "\" not implemented!");
    }

    ar << *set;

    return set;
}
