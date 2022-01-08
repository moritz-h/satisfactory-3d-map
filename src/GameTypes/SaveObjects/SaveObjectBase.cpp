#include "SaveObjectBase.h"

#include "SaveActor.h"
#include "SaveObject.h"
#include "Utils/StreamUtils.h"

std::shared_ptr<Satisfactory3DMap::SaveObjectBase> Satisfactory3DMap::SaveObjectBase::create(int32_t id,
    IStreamArchive& ar) {
    const auto type = ar.read_ahead<int32_t>();
    std::shared_ptr<SaveObjectBase> object;
    if (type == 0) { // object
        object = std::make_shared<SaveObject>(id);
    } else if (type == 1) { // actor
        object = std::make_shared<SaveActor>(id);
    } else {
        throw std::runtime_error("Unknown object type!");
    }
    ar << *object;
    return object;
}

Satisfactory3DMap::SaveObjectBase::SaveObjectBase(int32_t id) : id_(id) {}

void Satisfactory3DMap::SaveObjectBase::serialize(Archive& ar) {
    ar << type_;
    ar << class_name_;
    ar << reference_;
}

void Satisfactory3DMap::SaveObjectBase::parseData(int32_t length, std::istream& stream) {
    auto pos_before = stream.tellg();

    bool done = false;
    do {
        auto property = Property::parse(stream);
        if (property == nullptr) {
            done = true;
        } else {
            properties_.emplace_back(std::move(property));
        }
    } while (!done);

    // TODO unknown
    read_assert_zero<int32_t>(stream);

    auto pos_after = stream.tellg();

    // Read extras as binary buffer
    if (pos_after - pos_before != length) {
        extraProperties_ = read_vector<char>(stream, length - (pos_after - pos_before));
    }
}

void Satisfactory3DMap::SaveObjectBase::serializeData(std::ostream& stream) const {
    for (const auto& p : properties_) {
        p->serialize(stream);
    }
    // None property to terminate property list
    write_length_string(stream, "None");

    write<int32_t>(stream, 0);

    if (!extraProperties_.empty()) {
        stream.write(extraProperties_.data(), extraProperties_.size());
    }
}
