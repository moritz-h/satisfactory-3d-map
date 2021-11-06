#include "IntArray.h"

#include "ArrayVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::IntArray::IntArray(std::string array_type, int32_t count, std::istream& stream)
    : Array(std::move(array_type)) {
    array_ = read_vector<int32_t>(stream, count);
}

void Satisfactory3DMap::IntArray::serialize(std::ostream& stream) const {
    write(stream, static_cast<int32_t>(array_.size()));
    write_vector(stream, array_);
}

void Satisfactory3DMap::IntArray::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
