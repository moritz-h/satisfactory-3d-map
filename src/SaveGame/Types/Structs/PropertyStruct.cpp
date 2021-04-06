#include "PropertyStruct.h"

#include "StructVisitor.h"

Satisfactory3DMap::PropertyStruct::PropertyStruct(std::string struct_name, std::istream& stream)
    : Struct(std::move(struct_name)) {
    bool done = false;
    do {
        auto property = Property::parse(stream);
        if (property == nullptr) {
            done = true;
        } else {
            properties_.emplace_back(std::move(property));
        }
    } while (!done);
}

void Satisfactory3DMap::PropertyStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
