#include "StructArray.h"

#include "ArrayVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::StructArray::StructArray(std::string array_type, int32_t count, std::istream& stream)
    : Array(std::move(array_type)) {
    name_ = read_length_string(stream);
    auto type = read_length_string(stream);
    auto size = read<int32_t>(stream);
    auto index = read<int32_t>(stream);
    if (type != "StructProperty" || index != 0) {
        throw std::runtime_error("Invalid StructProperty array!");
    }

    struct_name_ = read_length_string(stream);
    guid_ = Guid(stream);
    read_assert_zero<int8_t>(stream);

    auto pos_before = stream.tellg();
    for (int32_t i = 0; i < count; i++) {
        array_.emplace_back(Struct::parse(struct_name_, stream));
    }
    auto pos_after = stream.tellg();
    if (pos_after - pos_before != size) {
        throw std::runtime_error("Invalid StructProperty array!");
    }
}

void Satisfactory3DMap::StructArray::serialize(std::ostream& stream) const {
    write(stream, static_cast<int32_t>(array_.size()));
    write_length_string(stream, name_);
    write_length_string(stream, "StructProperty");
    auto pos_size = stream.tellp();
    write<int32_t>(stream, 0);
    write<int32_t>(stream, 0);

    write_length_string(stream, struct_name_);
    guid_.serialize(stream);
    write<int8_t>(stream, 0);

    auto pos_before = stream.tellp();
    for (const auto& s : array_) {
        s->serialize(stream);
    }
    auto pos_after = stream.tellp();

    stream.seekp(pos_size);
    write(stream, static_cast<int32_t>(pos_after - pos_before));
    stream.seekp(pos_after);
}

void Satisfactory3DMap::StructArray::accept(Satisfactory3DMap::ArrayVisitor& v) {
    v.visit(*this);
}
