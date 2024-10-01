#include "GameTypes/Sets/StructSet.h"

#include "GameTypes/Sets/Base/SetVisitor.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

std::string SatisfactorySave::StructSet::structNameLookup(const FName& name, const std::string& parentClassName) {
    if (parentClassName == "/Script/FactoryGame.FGFoliageRemoval" && name == "mRemovalLocations") {
        return "Vector";
    } else if (parentClassName == "/Script/FactoryGame.FGScannableSubsystem" &&
               (name == "mDestroyedPickups" || name == "mLootedDropPods")) {
        return "Guid";
    } else {
        // Unknown struct types will be parsed as property struct anyway,
        // provide type information in struct name for debug logging.
        return "StructSet;" + parentClassName + ";" + name.toString();
    }
}

SatisfactorySave::StructSet::StructSet(FName struct_name) : struct_name_(std::move(struct_name)) {}

void SatisfactorySave::StructSet::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        const auto count = inAr.read<int32_t>();

        for (int32_t i = 0; i < count; i++) {
            Values.emplace_back(Struct::create(struct_name_, inAr));
        }
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        outAr.write(static_cast<int32_t>(Values.size()));

        for (auto& s : Values) {
            outAr << *s;
        }
    }
}
