#pragma once

#include <memory>
#include <type_traits>

namespace SatisfactorySave {
    template<typename T>
    concept IsUniquePtr = std::is_same_v<T, std::unique_ptr<typename T::element_type>>;
}
