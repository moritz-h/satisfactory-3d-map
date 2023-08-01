#include "GameTypes/Sets/StructSet.h"

#include "GameTypes/Sets/Base/SetVisitor.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

SatisfactorySave::StructSet::StructSet(const FName& name, const std::string& parentClassName) {
    if (parentClassName == "/Script/FactoryGame.FGFoliageRemoval" && name == "mRemovalLocations") {
        struct_name_.Name = "Vector";
    } else {
        // Unknown struct types will be parsed as property struct anyway,
        // provide type information in struct name for debug logging.
        struct_name_.Name = "StructSet;" + parentClassName + ";" + name.toString();
    }
}

void SatisfactorySave::StructSet::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        const auto count = inAr.read<int32_t>();

        for (int32_t i = 0; i < count; i++) {
            Set.emplace_back(Struct::create(struct_name_, inAr));
        }
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        outAr.write(static_cast<int32_t>(Set.size()));

        for (auto& s : Set) {
            outAr << *s;
        }
    }
}

void SatisfactorySave::StructSet::accept(SetVisitor& v) {
    v.visit(*this);
}
