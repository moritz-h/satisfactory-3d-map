#include "GameTypes/Sets/Base/Set.h"

#include <stdexcept>

#include "GameTypes/Sets/Base/SetAll.h"

std::shared_ptr<SatisfactorySave::Set> SatisfactorySave::Set::create(const FName& set_type, const FName& name,
    IStreamArchive& ar) {
    std::shared_ptr<Set> set;

    if (set_type == ObjectSet::TypeName) {
        set = std::make_shared<ObjectSet>();
    } else if (set_type == StructSet::TypeName) {
        auto struct_name = FName(StructSet::structNameLookup(name, ar.getParentClassInfo()));
        set = std::make_shared<StructSet>(std::move(struct_name));
    } else if (set_type == UInt32Set::TypeName) {
        set = std::make_shared<UInt32Set>();
    } else {
        throw std::runtime_error("Set type \"" + set_type + "\" not implemented!");
    }

    ar << *set;

    return set;
}
