#include "TextProperty.h"

#include <utility>

#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::TextProperty::TextProperty(PropertyTag tag, std::istream& stream) : Property(std::move(tag)) {
    buf_ = read_vector<char>(stream, tag_.Size); // TODO
}

void Satisfactory3DMap::TextProperty::serialize(std::ostream& stream) const {
    Property::serialize(stream);
    stream.write(buf_.data(), buf_.size()); // TODO
}

void Satisfactory3DMap::TextProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
