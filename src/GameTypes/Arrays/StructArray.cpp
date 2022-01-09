#include "StructArray.h"

#include "ArrayVisitor.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

void Satisfactory3DMap::StructArray::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        int32_t count = inAr.read<int32_t>();

        inAr << name_;
        std::string type;
        inAr << type;
        auto size = inAr.read<int32_t>();
        auto index = inAr.read<int32_t>();
        if (type != "StructProperty" || index != 0) {
            throw std::runtime_error("Invalid StructProperty array!");
        }

        inAr << struct_name_;
        inAr << guid_;
        inAr.read_assert_zero<int8_t>();

        auto pos_before = inAr.tell();
        for (int32_t i = 0; i < count; i++) {
            array_.emplace_back(Struct::create(struct_name_, inAr));
        }
        auto pos_after = inAr.tell();
        if (pos_after - pos_before != size) {
            throw std::runtime_error("Invalid StructProperty array!");
        }
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        outAr.write(static_cast<int32_t>(array_.size()));
        outAr << name_;
        std::string structProperty = "StructProperty";
        outAr << structProperty;
        auto pos_size = outAr.tell();
        outAr.write<int32_t>(0);
        outAr.write<int32_t>(0);

        outAr << struct_name_;
        outAr << guid_;
        outAr.write<int8_t>(0);

        auto pos_before = outAr.tell();
        for (auto& s : array_) {
            outAr << *s;
        }
        auto pos_after = outAr.tell();

        outAr.seek(pos_size);
        outAr.write(static_cast<int32_t>(pos_after - pos_before));
        outAr.seek(pos_after);
    }
}

void Satisfactory3DMap::StructArray::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
