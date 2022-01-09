#include "Properties.h"

#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

void Satisfactory3DMap::Properties::serialize(Satisfactory3DMap::Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        bool done = false;
        do {
            auto property = Property::create(inAr);
            if (property == nullptr) {
                done = true;
            } else {
                properties_.emplace_back(std::move(property));
            }
        } while (!done);
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        for (const auto& p : properties_) {
            auto& tag = p->tag();

            outAr << tag;

            auto pos_before = outAr.tell();

            outAr << *p;

            auto pos_after = outAr.tell();

            tag.Size = static_cast<int32_t>(pos_after - pos_before);
            outAr.seek(p->tag().SizeOffset);
            outAr << tag.Size;
            outAr.seek(pos_after);
        }
        // None property to terminate property list
        std::string none = "None";
        outAr << none;
    }
}
