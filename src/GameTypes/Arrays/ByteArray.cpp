#include "ByteArray.h"

#include "ArrayVisitor.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

void Satisfactory3DMap::ByteArray::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);
        auto count = inAr.read<int32_t>();
        array_ = inAr.read_vector<int8_t>(count);
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);
        outAr.write(static_cast<int32_t>(array_.size()));
        outAr.write_vector(array_);
    }
}

void Satisfactory3DMap::ByteArray::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
