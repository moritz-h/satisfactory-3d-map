#include "GameTypes/Sets/Base/Set.h"

#include <stdexcept>

#include "GameTypes/Sets/Base/SetAll.h"

std::unique_ptr<SatisfactorySave::Set> SatisfactorySave::Set::create(const FName& set_type, const FName& name,
    const std::string& parentClassName, Archive& ar) {
    std::unique_ptr<Set> set;

    if (set_type == StructSet::TypeName) {
        set = std::make_unique<StructSet>(name, parentClassName);
    } else if (set_type == UInt32Set::TypeName) {
        set = std::make_unique<UInt32Set>();
    } else {
        throw std::runtime_error("Set type \"" + set_type + "\" not implemented!");
    }

    ar << *set;

    return set;
}
