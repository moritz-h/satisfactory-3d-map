#include "GameTypes/Properties/Base/PropertyList.h"

#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

SatisfactorySave::PropertyList::PropertyList(const PropertyList& other) : std::vector<std::shared_ptr<Property>>() {
    this->reserve(other.size());
    for (const auto& p : other) {
        this->push_back(p->clone());
    }
}

SatisfactorySave::PropertyList& SatisfactorySave::PropertyList::operator=(const PropertyList& other) {
    if (this != &other) {
        this->clear();
        this->reserve(other.size());
        for (const auto& p : other) {
            this->push_back(p->clone());
        }
    }
    return *this;
}

void SatisfactorySave::PropertyList::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        bool done = false;
        do {
            auto property = Property::create(inAr);
            if (property == nullptr) {
                done = true;
            } else {
                this->push_back(std::move(property));
            }
        } while (!done);
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        for (const auto& p : *this) {
            outAr << p->tag_;

            auto pos_before = outAr.tell();

            outAr << *p;

            auto pos_after = outAr.tell();

            p->tag_.Size = static_cast<int32_t>(pos_after - pos_before);
            outAr.seek(p->tag_.SizeOffset);
            outAr << p->tag_.Size;
            outAr.seek(pos_after);
        }
        // None property to terminate property list
        std::string none = "None";
        outAr << none;
    }
}
