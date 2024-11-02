#pragma once

#include <memory>
#include <type_traits>

namespace SatisfactorySave {
    template<typename T>
    concept IsSharedPtr = std::is_same_v<T, std::shared_ptr<typename T::element_type>>;
}
