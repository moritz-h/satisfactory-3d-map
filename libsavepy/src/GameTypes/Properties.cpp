#include <pybind11/stl_bind.h>

#include "SatisfactorySave/GameTypes/Properties/Base/Property.h"
#include "SatisfactorySave/GameTypes/Properties/Base/PropertyAll.h"
#include "SatisfactorySave/GameTypes/Properties/Base/PropertyList.h"
#include "libsavepy_common.h"

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
    py::class_<s::Property, PyProperty, std::shared_ptr<s::Property>>(m, "Property")
        .def_property("Name",
            [](PyProperty& p) -> const s::FName& { return p.Name(); },
            [](PyProperty& p, const s::FName& v) { p.Name() = v; })
        .def_property_readonly("Type",
            [](PyProperty& p) -> const s::FName& { return p.Type(); })
        .def_property("ArrayIndex",
            [](PyProperty& p) -> int32_t { return p.ArrayIndex(); },
            [](PyProperty& p, int32_t v) { p.ArrayIndex() = v; })
        .def_property("HasPropertyGuid",
            [](PyProperty& p) -> uint8_t { return p.HasPropertyGuid(); },
            [](PyProperty& p, uint8_t v) { p.HasPropertyGuid() = v; })
        .def_property("PropertyGuid",
            [](PyProperty& p) -> const s::FGuid& { return p.PropertyGuid(); },
            [](PyProperty& p, const s::FGuid& v) { p.PropertyGuid() = v; });

    py::bind_vector<s::PropertyList>(m, "PropertyList")
        .def("get",
            [](s::PropertyList& l, const std::string& name) -> std::shared_ptr<s::Property> {
                return l.getPtr(name);
            });

    py::class_<s::ArrayProperty, s::Property, std::shared_ptr<s::ArrayProperty>>(m, "ArrayProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::ArrayProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_property("ArrayType",
            [](s::ArrayProperty& p) -> s::FName& { return p.ArrayType(); },
            [](s::ArrayProperty& p, const s::FName& v) { p.ArrayType() = v; })
        .def_readwrite("Value", &s::ArrayProperty::Value);

    py::class_<s::BoolProperty, s::Property, std::shared_ptr<s::BoolProperty>>(m, "BoolProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::BoolProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_property("Value", &s::BoolProperty::getValue, &s::BoolProperty::setValue);

    py::class_<s::ByteProperty, s::Property, std::shared_ptr<s::ByteProperty>>(m, "ByteProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::ByteProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_property("EnumName",
            [](s::ByteProperty& p) -> s::FName& { return p.EnumName(); },
            [](s::ByteProperty& p, const s::FName& v) { p.EnumName() = v; })
        .def_readwrite("Value", &s::ByteProperty::Value);

    py::class_<s::DoubleProperty, s::Property, std::shared_ptr<s::DoubleProperty>>(m, "DoubleProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::DoubleProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::DoubleProperty::Value);

    py::class_<s::EnumProperty, s::Property, std::shared_ptr<s::EnumProperty>>(m, "EnumProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::EnumProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_property("EnumName",
            [](s::EnumProperty& p) -> s::FName& { return p.EnumName(); },
            [](s::EnumProperty& p, const s::FName& v) { p.EnumName() = v; })
        .def_readwrite("Value", &s::EnumProperty::Value);

    py::class_<s::FloatProperty, s::Property, std::shared_ptr<s::FloatProperty>>(m, "FloatProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::FloatProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::FloatProperty::Value);

    py::class_<s::Int8Property, s::Property, std::shared_ptr<s::Int8Property>>(m, "Int8Property")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::Int8Property::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::Int8Property::Value);

    py::class_<s::Int64Property, s::Property, std::shared_ptr<s::Int64Property>>(m, "Int64Property")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::Int64Property::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::Int64Property::Value);

    py::class_<s::IntProperty, s::Property, std::shared_ptr<s::IntProperty>>(m, "IntProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::IntProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::IntProperty::Value);

    py::class_<s::MapProperty, s::Property, std::shared_ptr<s::MapProperty>>(m, "MapProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, std::shared_ptr<s::MapTypeList>, std::shared_ptr<s::MapTypeList>>(), py::arg("name"), py::arg("keys"), py::arg("values"))
        .def_property("KeyType",
            [](s::MapProperty& p) -> s::FName& { return p.KeyType(); },
            [](s::MapProperty& p, const s::FName& v) { p.KeyType() = v; })
        .def_property("ValueType",
            [](s::MapProperty& p) -> s::FName& { return p.ValueType(); },
            [](s::MapProperty& p, const s::FName& v) { p.ValueType() = v; })
        .def_readwrite("Keys", &s::MapProperty::Keys)
        .def_readwrite("Values", &s::MapProperty::Values);

    py::class_<s::MulticastSparseDelegateProperty, s::Property, std::shared_ptr<s::MulticastSparseDelegateProperty>>(m, "MulticastSparseDelegateProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::MulticastSparseDelegateProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::MulticastSparseDelegateProperty::Value);

    py::class_<s::NameProperty, s::Property, std::shared_ptr<s::NameProperty>>(m, "NameProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::NameProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::NameProperty::Value);

    py::class_<s::ObjectProperty, s::Property, std::shared_ptr<s::ObjectProperty>>(m, "ObjectProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::ObjectProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::ObjectProperty::Value);

    py::class_<s::SetProperty, s::Property, std::shared_ptr<s::SetProperty>>(m, "SetProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::SetProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_property("SetType",
            [](s::SetProperty& p) -> s::FName& { return p.SetType(); },
            [](s::SetProperty& p, const s::FName& v) { p.SetType() = v; })
        .def_readwrite("Value", &s::SetProperty::Value);

    py::class_<s::SoftObjectProperty, s::Property, std::shared_ptr<s::SoftObjectProperty>>(m, "SoftObjectProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::SoftObjectProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::SoftObjectProperty::Value);

    py::class_<s::StrProperty, s::Property, std::shared_ptr<s::StrProperty>>(m, "StrProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::StrProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::StrProperty::Value);

    py::class_<s::StructProperty, s::Property, std::shared_ptr<s::StructProperty>>(m, "StructProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::StructProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_property("StructName",
            [](s::StructProperty& p) -> s::FName& { return p.StructName(); },
            [](s::StructProperty& p, const s::FName& v) { p.StructName() = v; })
        .def_property("StructGuid",
            [](s::StructProperty& p) -> s::FGuid& { return p.StructGuid(); },
            [](s::StructProperty& p, const s::FGuid& v) { p.StructGuid() = v; })
        .def_readwrite("Value", &s::StructProperty::Value);

    py::class_<s::TextProperty, s::Property, std::shared_ptr<s::TextProperty>>(m, "TextProperty")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::TextProperty::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::TextProperty::Value);

    py::class_<s::UInt32Property, s::Property, std::shared_ptr<s::UInt32Property>>(m, "UInt32Property")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::UInt32Property::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::UInt32Property::Value);

    py::class_<s::UInt64Property, s::Property, std::shared_ptr<s::UInt64Property>>(m, "UInt64Property")
        .def(py::init<>())
        .def(py::init<s::FName>(), py::arg("name"))
        .def(py::init<s::FName, s::UInt64Property::value_type>(), py::arg("name"), py::arg("value"))
        .def_readwrite("Value", &s::UInt64Property::Value);

    py::class_<s::UnknownProperty, s::Property, std::shared_ptr<s::UnknownProperty>>(m, "UnknownProperty")
        .def(py::init<s::FName>())
        .def_property("Value",
            [](s::UnknownProperty& p) -> py::bytes { return {p.Value.data(), p.Value.size()}; },
            [](s::UnknownProperty& p, const std::string& v) { p.Value = std::vector<char>(v.begin(), v.end()); });
}
