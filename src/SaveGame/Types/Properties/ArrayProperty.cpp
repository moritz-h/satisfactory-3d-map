#include "ArrayProperty.h"

#include <utility>

#include "SaveGame/Types/Guid.h"
#include "SaveGame/Types/ObjectReference.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::ArrayProperty::ArrayProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    array_type_ = read_length_string(stream);
    read_assert_zero<int8_t>(stream);

    auto count = read<int32_t>(stream);

    if (array_type_ == "ByteProperty") {
        read_vector<int8_t>(stream, count);
    } else if (array_type_ == "EnumProperty") {
        for (int32_t i = 0; i < count; i++) {
            read_length_string(stream);
        }
    } else if (array_type_ == "InterfaceProperty") {
        for (int32_t i = 0; i < count; i++) {
            ObjectReference ref(stream);
        }
    } else if (array_type_ == "IntProperty") {
        read_vector<int32_t>(stream, count);
    } else if (array_type_ == "ObjectProperty") {
        for (int32_t i = 0; i < count; i++) {
            ObjectReference ref(stream);
        }
    } else if (array_type_ == "StructProperty") {
        auto name = read_length_string(stream);
        auto type = read_length_string(stream);
        auto size = read<int32_t>(stream);
        auto index = read<int32_t>(stream);
        if (type != "StructProperty") {
            throw std::runtime_error("Invalid StructProperty array!");
        }

        auto structName = read_length_string(stream);
        Guid guid(stream);
        read_assert_zero<int8_t>(stream);

        for (int32_t i = 0; i < count; i++) {
            if (structName == "LinearColor") {
                auto r = read<float>(stream);
                auto g = read<float>(stream);
                auto b = read<float>(stream);
                auto a = read<float>(stream);
            } else if (structName == "FeetOffset" || structName == "Hotbar" || structName == "InventoryStack" ||
                       structName == "ItemAmount" || structName == "ItemFoundData" || structName == "MessageData" ||
                       structName == "PhaseCost" || structName == "RemovedInstance" || structName == "SpawnData" ||
                       structName == "SplinePointData" || structName == "SplitterSortRule" ||
                       structName == "TimeTableStop") {
                std::vector<std::unique_ptr<Property>> properties;
                bool done = false;
                do {
                    auto property = Property::parse(stream);
                    if (property == nullptr) {
                        done = true;
                    } else {
                        properties.emplace_back(std::move(property));
                    }
                } while (!done);
            } else {
                throw std::runtime_error("Struct array \"" + structName + "\" not implemented!");
            }
        }
    } else {
        throw std::runtime_error("Array type \"" + array_type_ + "\" not implemented!");
    }
}
