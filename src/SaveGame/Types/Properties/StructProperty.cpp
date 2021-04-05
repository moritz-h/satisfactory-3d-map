#include "StructProperty.h"

#include <utility>

#include <glm/glm.hpp>

#include "../ObjectReference.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::StructProperty::StructProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    struct_name_ = read_length_string(stream);
    guid_ = Guid(stream);
    read_assert_zero<int8_t>(stream);

    if (struct_name_ == "Box") {
        auto min = read<glm::vec3>(stream);
        auto max = read<glm::vec3>(stream);
        auto isValid = read<uint8_t>(stream);
    } else if (struct_name_ == "Color") {
        auto b = read<int8_t>(stream);
        auto g = read<int8_t>(stream);
        auto r = read<int8_t>(stream);
        auto a = read<int8_t>(stream);
    } else if (struct_name_ == "FluidBox") {
        auto value = read<float>(stream);
    } else if (struct_name_ == "InventoryItem") {
        auto unk1 = read<int32_t>(stream);
        auto className = read_length_string(stream);
        ObjectReference ref(stream);
    } else if (struct_name_ == "LinearColor") {
        auto r = read<float>(stream);
        auto g = read<float>(stream);
        auto b = read<float>(stream);
        auto a = read<float>(stream);
    } else if (struct_name_ == "Quat") {
        auto x = read<float>(stream);
        auto y = read<float>(stream);
        auto z = read<float>(stream);
        auto w = read<float>(stream);
    } else if (struct_name_ == "RailroadTrackPosition") {
        ObjectReference ref(stream);
        auto offset = read<float>(stream);
        auto forward = read<float>(stream);
    } else if (struct_name_ == "Vector") {
        auto vec = read<glm::vec3>(stream);
    } else if (struct_name_ == "InventoryStack" || struct_name_ == "RemovedInstanceArray" ||
               struct_name_ == "TimerHandle" || struct_name_ == "TrainSimulationData" || struct_name_ == "Transform") {
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
        throw std::runtime_error("Struct name \"" + struct_name_ + "\" not implemented!");
    }
}
