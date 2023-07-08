#include "StructSet.h"

#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"
#include "SetVisitor.h"

Satisfactory3DMap::StructSet::StructSet(FName type, const FName& name, const std::string& parentClassName)
    : Set(std::move(type)) {
    if (parentClassName == "/Script/FactoryGame.FGFoliageRemoval" && name == "mRemovalLocations") {
        struct_name_.Name = "Vector";
    } else {
        // Unknown struct types will be parsed as property struct anyway,
        // provide type information in struct name for debug logging.
        struct_name_.Name = "StructSet;" + parentClassName + ";" + name.toString();
    }
}

void Satisfactory3DMap::StructSet::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        const auto count = inAr.read<int32_t>();

        for (int32_t i = 0; i < count; i++) {
            set_.emplace_back(Struct::create(struct_name_, inAr));
        }
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        outAr.write(static_cast<int32_t>(set_.size()));

        for (auto& s : set_) {
            outAr << *s;
        }
    }
}

void Satisfactory3DMap::StructSet::accept(SetVisitor& v) {
    v.visit(*this);
}
