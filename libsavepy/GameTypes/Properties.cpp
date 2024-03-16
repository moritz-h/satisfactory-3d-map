#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SatisfactorySave/GameTypes/Properties/Base/Property.h"
#include "SatisfactorySave/GameTypes/Properties/Base/PropertyAll.h"
#include "SatisfactorySave/GameTypes/Properties/Base/PropertyList.h"
#include "libsavepy_common.h"
#include "vector_bind_utils.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

namespace {
    class PyProperty : public s::Property {
    public:
        using s::Property::Property;

        void serialize(s::Archive& ar) override {
            PYBIND11_OVERRIDE_PURE(void, s::Property, serialize, ar);
        }

        void accept(s::PropertyVisitor& v) override {
            PYBIND11_OVERRIDE_PURE(void, s::Property, accept, v);
        }
    };
} // namespace

void init_GameTypes_Properties(py::module_& m) {
    py::class_<s::Property, PyProperty>(m, "Property")
        .def_property("Name",
            [](PyProperty& p) -> const s::FName& { return p.name(); },
            [](PyProperty& p, const s::FName& v) { p.name() = v; })
        .def_property_readonly("Type",
            [](PyProperty& p) -> const s::FName& { return p.type(); })
        .def_property("ArrayIndex",
            [](PyProperty& p) -> int32_t { return p.arrayIndex(); },
            [](PyProperty& p, int32_t v) { p.arrayIndex() = v; })
        .def_property("HasPropertyGuid",
            [](PyProperty& p) -> uint8_t { return p.hasPropertyGuid(); },
            [](PyProperty& p, uint8_t v) { p.hasPropertyGuid() = v; })
        .def_property("PropertyGuid",
            [](PyProperty& p) -> const s::FGuid& { return p.propertyGuid(); },
            [](PyProperty& p, const s::FGuid& v) { p.propertyGuid() = v; });

    bind_vector_unique_ptr<s::PropertyList>(m, "PropertyList")
        .def("get",
            [](s::PropertyList& l, const std::string& name) -> s::Property& {
                return *l.getPtr(name);
            });

    py::class_<s::ArrayProperty, s::Property>(m, "ArrayProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init([](s::FName name, const s::Array& v) {
            return s::ArrayProperty{std::move(name), std::move(v.clone())};
        }), py::arg("name"), py::arg("value"))
        .def_property("ArrayType",
            [](s::ArrayProperty& p) -> const s::FName& { return p.arrayType(); },
            [](s::ArrayProperty& p, const s::FName& v) { p.arrayType() = v; })
        .def_property("Value",
            [](s::ArrayProperty& p) -> const s::Array& { return *p.Value; },
            [](s::ArrayProperty& p, const s::Array& v) { p.Value = v.clone(); });

    py::class_<s::BoolProperty, s::Property>(m, "BoolProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::BoolProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_property("Value", &s::BoolProperty::getValue, &s::BoolProperty::setValue);

    py::class_<s::ByteProperty, s::Property>(m, "ByteProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::ByteProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_property("EnumName",
            [](s::ByteProperty& p) -> const s::FName& { return p.enumName(); },
            [](s::ByteProperty& p, const s::FName& v) { p.enumName() = v; })
        .def_readwrite("Value", &s::ByteProperty::Value);

    py::class_<s::DoubleProperty, s::Property>(m, "DoubleProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::DoubleProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::DoubleProperty::Value);

    py::class_<s::EnumProperty, s::Property>(m, "EnumProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::EnumProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_property("EnumName",
            [](s::EnumProperty& p) -> const s::FName& { return p.enumName(); },
            [](s::EnumProperty& p, const s::FName& v) { p.enumName() = v; })
        .def_readwrite("Value", &s::EnumProperty::Value);

    py::class_<s::FloatProperty, s::Property>(m, "FloatProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::FloatProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::FloatProperty::Value);

    py::class_<s::Int8Property, s::Property>(m, "Int8Property")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::Int8Property::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::Int8Property::Value);

    py::class_<s::Int64Property, s::Property>(m, "Int64Property")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::Int64Property::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::Int64Property::Value);

    py::class_<s::IntProperty, s::Property>(m, "IntProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::IntProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::IntProperty::Value);

    py::class_<s::MapProperty, s::Property>(m, "MapProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init([](s::FName name, const s::MapTypeList& k, const s::MapTypeList& v) {
            return s::MapProperty{std::move(name), std::move(k.clone()), std::move(v.clone())};
        }), py::arg("name"), py::arg("keys"), py::arg("values"))
        .def_property("KeyType",
            [](s::MapProperty& p) -> const s::FName& { return p.keyType(); },
            [](s::MapProperty& p, const s::FName& v) { p.keyType() = v; })
        .def_property("ValueType",
            [](s::MapProperty& p) -> const s::FName& { return p.valueType(); },
            [](s::MapProperty& p, const s::FName& v) { p.valueType() = v; })
        .def_property("Keys",
            [](s::MapProperty& p) -> const s::MapTypeList& { return *p.Keys; },
            [](s::MapProperty& p, const s::MapTypeList& v) { p.Keys = v.clone(); })
        .def_property("Values",
            [](s::MapProperty& p) -> const s::MapTypeList& { return *p.Values; },
            [](s::MapProperty& p, const s::MapTypeList& v) { p.Values = v.clone(); });

    py::class_<s::MulticastSparseDelegateProperty, s::Property>(m, "MulticastSparseDelegateProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::MulticastSparseDelegateProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::MulticastSparseDelegateProperty::Value);

    py::class_<s::NameProperty, s::Property>(m, "NameProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::NameProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::NameProperty::Value);

    py::class_<s::ObjectProperty, s::Property>(m, "ObjectProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::ObjectProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::ObjectProperty::Value);

    py::class_<s::SetProperty, s::Property>(m, "SetProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init([](s::FName name, const s::Set& v) {
            return s::SetProperty{std::move(name), std::move(v.clone())};
        }), py::arg("name"), py::arg("value"))
        .def_property("SetType",
            [](s::SetProperty& p) -> const s::FName& { return p.setType(); },
            [](s::SetProperty& p, const s::FName& v) { p.setType() = v; })
        .def_property("Value",
            [](s::SetProperty& p) -> const s::Set& { return *p.Value; },
            [](s::SetProperty& p, const s::Set& v) { p.Value = v.clone(); });

    py::class_<s::SoftObjectProperty, s::Property>(m, "SoftObjectProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::SoftObjectProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::SoftObjectProperty::Value);

    py::class_<s::StrProperty, s::Property>(m, "StrProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::StrProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::StrProperty::Value);

    py::class_<s::StructProperty, s::Property>(m, "StructProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init([](s::FName name, const s::Struct& v) {
            return s::StructProperty{std::move(name), std::move(v.clone())};
        }), py::arg("name"), py::arg("value"))
        .def_property("StructName",
            [](s::StructProperty& p) -> const s::FName& { return p.structName(); },
            [](s::StructProperty& p, const s::FName& v) { p.structName() = v; })
        .def_property("StructGuid",
            [](s::StructProperty& p) -> const s::FGuid& { return p.structGuid(); },
            [](s::StructProperty& p, const s::FGuid& v) { p.structGuid() = v; })
        .def_property("Value",
            [](s::StructProperty& p) -> const s::Struct& { return *p.Value; },
            [](s::StructProperty& p, const s::Struct& v) { p.Value = v.clone(); });

    py::class_<s::TextProperty, s::Property>(m, "TextProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::TextProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::TextProperty::Value);

    py::class_<s::UInt32Property, s::Property>(m, "UInt32Property")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::UInt32Property::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::UInt32Property::Value);

    py::class_<s::UInt64Property, s::Property>(m, "UInt64Property")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::UInt64Property::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::UInt64Property::Value);

    py::class_<s::UnknownProperty, s::Property>(m, "UnknownProperty")
        .def(py::init<s::FName>())
        .def_property("Value",
            [](s::UnknownProperty& p) -> py::bytes { return {p.Value.data(), p.Value.size()}; },
            [](s::UnknownProperty& p, const std::string& v) { p.Value = std::vector<char>(v.begin(), v.end()); });
}
