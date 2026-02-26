#include "GameTypes/Arrays/StructArray.h"

#include "GameTypes/Arrays/Base/ArrayVisitor.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

SatisfactorySave::StructArray::StructArray(const FPropertyTypeName& type_param) : type_param_(type_param) {
    inner_tag_.Type = "StructProperty";
    inner_tag_.ArrayIndex = 0;
}

SatisfactorySave::StructArray::StructArray(const StructArray& other) : Array(other) {
    Values.reserve(other.Values.size());
    for (const auto& v : other.Values) {
        Values.push_back(v->clone());
    }
    inner_tag_ = other.inner_tag_;
    type_param_ = other.type_param_;
}

SatisfactorySave::StructArray& SatisfactorySave::StructArray::operator=(const StructArray& other) {
    if (this != &other) {
        Array::operator=(other);
        Values.clear();
        Values.reserve(other.Values.size());
        for (const auto& v : other.Values) {
            Values.push_back(v->clone());
        }
        inner_tag_ = other.inner_tag_;
        type_param_ = other.type_param_;
    }
    return *this;
}

std::shared_ptr<SatisfactorySave::Array> SatisfactorySave::StructArray::clone() const {
    return std::make_shared<StructArray>(*this);
}

void SatisfactorySave::StructArray::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        int32_t count = inAr.read<int32_t>();

        if (ar.getSaveVersion() >= 53) {
            if (type_param_.IsEmpty()) {
                throw std::runtime_error("Missing FPropertyTypeName in StructArray!");
            }
            inner_tag_.Type = type_param_.GetName();
            inner_tag_.StructName = type_param_.GetParameterName(0);
        } else {
            // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/PropertyArray.cpp#L183
            inAr << inner_tag_;
        }

        if (inner_tag_.Type != "StructProperty" || inner_tag_.ArrayIndex != 0) {
            throw std::runtime_error("Invalid StructProperty array!");
        }

        auto pos_before = inAr.tell();
        for (int32_t i = 0; i < count; i++) {
            Values.emplace_back(Struct::create(inner_tag_.StructName, inAr));
        }
        auto pos_after = inAr.tell();
        if (ar.getSaveVersion() < 53 && pos_after - pos_before != inner_tag_.Size) {
            throw std::runtime_error("Invalid StructProperty array!");
        }
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        outAr.write(static_cast<int32_t>(Values.size()));
        if (ar.getSaveVersion() < 53) {
            outAr << inner_tag_;
        }

        auto pos_before = outAr.tell();
        for (auto& s : Values) {
            outAr << *s;
        }
        auto pos_after = outAr.tell();

        if (ar.getSaveVersion() < 53) {
            outAr.seek(inner_tag_.SizeOffset);
            outAr.write(static_cast<int32_t>(pos_after - pos_before));
            outAr.seek(pos_after);
        }
    }
}

void SatisfactorySave::StructArray::accept(ArrayVisitor& v) {
    v.visit(*this);
}
