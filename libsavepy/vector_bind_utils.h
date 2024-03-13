#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;

template<typename Vector>
py::class_<Vector> bind_vector_unique_ptr(py::handle scope, std::string const& name) {
    // TODO check in concept that Vector is std::vector<std::unique_ptr<T>>

    using T = typename Vector::value_type::element_type;
    using SizeType = typename Vector::size_type;
    using DiffType = typename Vector::difference_type;
    using ItType = typename Vector::iterator;

    auto wrap_i = [](DiffType i, SizeType n) {
        if (i < 0) {
            i += n;
        }
        if (i < 0 || (SizeType) i >= n) {
            throw py::index_error();
        }
        return i;
    };

    py::class_<Vector> cl(scope, name.c_str());
    cl.def(py::init<>());
    // cl.def("count", ...);
    // cl.def("remove", ...);
    // cl.def("__contains__", ...);
    cl.def(
        "__repr__",
        [name](Vector& v) {
            std::ostringstream s;
            s << name << '[';
            for (SizeType i = 0; i < v.size(); i++) {
                s << "obj[" << i << "]"; // TODO
                if (i != v.size() - 1) {
                    s << ", ";
                }
            }
            s << ']';
            return s.str();
        },
        "Return the canonical string representation of this list.");
    cl.def(
        "append",
        [](Vector& v, const T& x) { v.push_back(std::move(x.clone())); },
        py::arg("x"),
        "Add an item to the end of the list.");
    cl.def(py::init([](const py::iterable& it) {
        auto v = std::unique_ptr<Vector>(new Vector());
        v->reserve(len_hint(it));
        for (py::handle h : it) {
            v->push_back(std::move(h.cast<T*>()->clone()));
        }
        return v.release();
    }));
    cl.def(
        "clear",
        [](Vector& v) { v.clear(); },
        "Clear the contents.");
    cl.def(
        "extend",
        [](Vector& v, const Vector& src) {
            v.reserve(v.size() + src.size());
            for (const auto& s : src) {
                v.push_back(std::move(s->clone()));
            }
        },
        py::arg("L"),
        "Extend the list by appending all the items in the given list");
    cl.def(
        "extend",
        [](Vector& v, const py::iterable& it) {
            const size_t old_size = v.size();
            v.reserve(old_size + len_hint(it));
            try {
                for (py::handle h : it) {
                    v.push_back(std::move(h.cast<T*>()->clone()));
                }
            } catch (const py::cast_error&) {
                v.erase(v.begin() + static_cast<typename Vector::difference_type>(old_size), v.end());
                try {
                    v.shrink_to_fit();
                } catch (const std::exception&) {
                    // Do nothing
                }
                throw;
            }
        },
        py::arg("L"),
        "Extend the list by appending all the items in the given list");
    cl.def(
        "insert",
        [](Vector& v, DiffType i, const T& x) {
            if (i < 0) {
                i += v.size();
            }
            if (i < 0 || static_cast<SizeType>(i) > v.size()) {
                throw py::index_error();
            }
            v.insert(v.begin() + i, x.clone());
        },
        py::arg("i"),
        py::arg("x"),
        "Insert an item at a given position.");
    cl.def(
        "pop",
        [](Vector& v) {
            if (v.empty()) {
                throw py::index_error();
            }
            std::unique_ptr<T> t = std::move(v.back());
            v.pop_back();
            return t;
        },
        "Remove and return the last item");
    cl.def(
        "pop",
        [wrap_i](Vector& v, DiffType i) {
            i = wrap_i(i, v.size());
            std::unique_ptr<T> t = std::move(v[(SizeType) i]);
            v.erase(std::next(v.begin(), i));
            return t;
        },
        py::arg("i"),
        "Remove and return the item at index ``i``");
    cl.def(
        "__setitem__",
        [wrap_i](Vector& v, DiffType i, const T& t) {
            i = wrap_i(i, v.size());
            v[(SizeType) i] = t.clone();
        });
    cl.def(
        "__getitem__",
        [](const Vector& v, const py::slice& slice) -> Vector* {
            size_t start = 0, stop = 0, step = 0, slicelength = 0;

            if (!slice.compute(v.size(), &start, &stop, &step, &slicelength)) {
                throw py::error_already_set();
            }

            auto* seq = new Vector();
            seq->reserve((size_t) slicelength);

            for (size_t i = 0; i < slicelength; ++i) {
                seq->push_back(std::move(v[start]->clone()));
                start += step;
            }
            return seq;
        },
        py::arg("s"),
        "Retrieve list elements using a slice object");
    cl.def(
        "__setitem__",
        [](Vector& v, const py::slice& slice, const Vector& value) {
            size_t start = 0, stop = 0, step = 0, slicelength = 0;
            if (!slice.compute(v.size(), &start, &stop, &step, &slicelength)) {
                throw py::error_already_set();
            }

            if (slicelength != value.size()) {
                throw std::runtime_error("Left and right hand size of slice assignment have different sizes!");
            }

            for (size_t i = 0; i < slicelength; ++i) {
                v[start] = value[i]->clone();
                start += step;
            }
        },
        "Assign list elements using a slice object");
    cl.def(
        "__delitem__",
        [wrap_i](Vector& v, DiffType i) {
            i = wrap_i(i, v.size());
            v.erase(v.begin() + i);
        },
        "Delete the list elements at index ``i``");
    cl.def(
        "__delitem__",
        [](Vector& v, const py::slice& slice) {
            size_t start = 0, stop = 0, step = 0, slicelength = 0;

            if (!slice.compute(v.size(), &start, &stop, &step, &slicelength)) {
                throw py::error_already_set();
            }

            if (step == 1 && false) {
                v.erase(v.begin() + (DiffType) start, v.begin() + DiffType(start + slicelength));
            } else {
                for (size_t i = 0; i < slicelength; ++i) {
                    v.erase(v.begin() + DiffType(start));
                    start += step - 1;
                }
            }
        },
        "Delete list elements using a slice object");
    cl.def(
        "__getitem__",
        [](const Vector& v, DiffType i) -> std::unique_ptr<T> {
            if (i < 0) {
                i += v.size();
                if (i < 0) {
                    throw py::index_error();
                }
            }
            auto i_st = static_cast<SizeType>(i);
            if (i_st >= v.size()) {
                throw py::index_error();
            }
            return v[i_st]->clone();
        });
    // cl.def("__iter__", ...);
    cl.def(
        "__bool__",
        [](const Vector& vec) -> bool { return !vec.empty(); },
        "Check whether the list is nonempty.");
    cl.def(
        "__len__",
        [](const Vector& vec) -> std::size_t { return vec.size(); });
    return cl;
}
