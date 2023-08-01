#include "GameTypes/Properties/Base/PropertyList.h"

#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

void SatisfactorySave::PropertyList::serialize(SatisfactorySave::Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        bool done = false;
        do {
            auto property = Property::create(inAr, parentClassName_);
            if (property == nullptr) {
                done = true;
            } else {
                properties_.emplace_back(std::move(property));
            }
        } while (!done);
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        for (const auto& p : properties_) {
            outAr << p->Tag;

            auto pos_before = outAr.tell();

            outAr << *p;

            auto pos_after = outAr.tell();

            p->Tag.Size = static_cast<int32_t>(pos_after - pos_before);
            outAr.seek(p->Tag.SizeOffset);
            outAr << p->Tag.Size;
            outAr.seek(pos_after);
        }
        // None property to terminate property list
        std::string none = "None";
        outAr << none;
    }
}
