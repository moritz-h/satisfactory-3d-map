#pragma once

#include <cstddef>
#include <cstring>
#include <string_view>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

inline py::bytes bytes_to_py(const std::vector<std::byte>& b) {
    return {reinterpret_cast<const char*>(b.data()), b.size()};
}

inline void bytes_from_py(std::vector<std::byte>& out, const py::bytes& b) {
    const std::string_view sv = b;
    out.resize(sv.size());
    if (!sv.empty()) {
        std::memcpy(out.data(), sv.data(), sv.size());
    }
}
