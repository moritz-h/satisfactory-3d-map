#include "GameTypes/Arrays/StructArray.h"

#include "GameTypes/Arrays/Base/ArrayVisitor.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

void SatisfactorySave::StructArray::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        int32_t count = inAr.read<int32_t>();

        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/PropertyArray.cpp#L183
        inAr << inner_tag_;

        if (inner_tag_.Type != "StructProperty" || inner_tag_.ArrayIndex != 0) {
            throw std::runtime_error("Invalid StructProperty array!");
        }

        auto pos_before = inAr.tell();
        for (int32_t i = 0; i < count; i++) {
            Values.emplace_back(Struct::create(inner_tag_.StructName, inAr));
        }
        auto pos_after = inAr.tell();
        if (pos_after - pos_before != inner_tag_.Size) {
            throw std::runtime_error("Invalid StructProperty array!");
        }
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        outAr.write(static_cast<int32_t>(Values.size()));
        outAr << inner_tag_;

        auto pos_before = outAr.tell();
        for (auto& s : Values) {
            outAr << *s;
        }
        auto pos_after = outAr.tell();

        outAr.seek(inner_tag_.SizeOffset);
        outAr.write(static_cast<int32_t>(pos_after - pos_before));
        outAr.seek(pos_after);
    }
}

void SatisfactorySave::StructArray::accept(SatisfactorySave::ArrayVisitor& v) {
    v.visit(*this);
}
