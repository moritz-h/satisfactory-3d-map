#include "TextProperty.h"

#include "IO/Archive/OStreamArchive.h"
#include "PropertyVisitor.h"

void Satisfactory3DMap::TextProperty::serialize(Archive& ar) {
    Property::serialize(ar);

    // TODO
    if (ar.isIArchive()) {
        buf_ = dynamic_cast<IStreamArchive&>(ar).read_vector<char>(tag_.Size);
    } else {
        dynamic_cast<OStreamArchive&>(ar).write_vector(buf_);
    }
}

void Satisfactory3DMap::TextProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
