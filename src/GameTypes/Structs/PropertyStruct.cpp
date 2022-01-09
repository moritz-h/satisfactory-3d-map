#include "PropertyStruct.h"

#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"
#include "StructVisitor.h"

void Satisfactory3DMap::PropertyStruct::serialize(Archive& ar) {
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
            outAr << *p;
        }
        // None property to terminate property list
        std::string none = "None";
        outAr << none;
    }
}

void Satisfactory3DMap::PropertyStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
