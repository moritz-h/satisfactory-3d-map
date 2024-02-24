#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SatisfactorySave/GameTypes/Properties/Base/Property.h"
#include "SatisfactorySave/GameTypes/Properties/Base/PropertyAll.h"
#include "SatisfactorySave/GameTypes/Properties/Base/PropertyList.h"
#include "SatisfactorySave/GameTypes/Properties/Base/PropertyTag.h"
#include "libsavepy_common.h"

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
    py::class_<s::PropertyTag>(m, "PropertyTag")
        .def(py::init<>())
        .def_readwrite("Name", &s::PropertyTag::Name)
        .def_readwrite("Type", &s::PropertyTag::Type)
        .def_readwrite("Size", &s::PropertyTag::Size)
        .def_readwrite("ArrayIndex", &s::PropertyTag::ArrayIndex)
        .def_readwrite("StructName", &s::PropertyTag::StructName)
        .def_readwrite("StructGuid", &s::PropertyTag::StructGuid)
        .def_readwrite("BoolVal", &s::PropertyTag::BoolVal)
        .def_readwrite("EnumName", &s::PropertyTag::EnumName)
        .def_readwrite("InnerType", &s::PropertyTag::InnerType)
        .def_readwrite("ValueType", &s::PropertyTag::ValueType)
        .def_readwrite("HasPropertyGuid", &s::PropertyTag::HasPropertyGuid)
        .def_readwrite("PropertyGuid", &s::PropertyTag::PropertyGuid);

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

    py::class_<s::PropertyList>(m, "PropertyList")
        //.def("properties", &s::PropertyList::properties) // TODO
        .def("at", &s::PropertyList::at)
        // TODO get()
        ;

    py::class_<s::ArrayProperty, s::Property>(m, "ArrayProperty")
        .def(py::init<s::PropertyTag>())
        //.def_readwrite("Value", &s::ArrayProperty::Value) // TODO
        ;

    py::class_<s::BoolProperty, s::Property>(m, "BoolProperty")
        .def(py::init<s::PropertyTag>())
        .def_property("Value", &s::BoolProperty::getValue, &s::BoolProperty::setValue);

    py::class_<s::ByteProperty, s::Property>(m, "ByteProperty")
        .def(py::init<s::PropertyTag>())
        .def("enumName", &s::ByteProperty::enumName)
        .def("valueName", &s::ByteProperty::valueName)
        .def("valueByte", &s::ByteProperty::valueByte);

    py::class_<s::DoubleProperty, s::Property>(m, "DoubleProperty")
        .def(py::init<>())
        .def_readwrite("Value", &s::DoubleProperty::Value);

    py::class_<s::EnumProperty, s::Property>(m, "EnumProperty")
        .def(py::init<s::PropertyTag>())
        .def_readwrite("Value", &s::EnumProperty::Value)
        .def("enumName", &s::EnumProperty::enumName);

    py::class_<s::FloatProperty, s::Property>(m, "FloatProperty")
        .def(py::init<>())
        .def_readwrite("Value", &s::FloatProperty::Value);

    py::class_<s::Int8Property, s::Property>(m, "Int8Property")
        .def(py::init<>())
        .def_readwrite("Value", &s::Int8Property::Value);

    py::class_<s::Int64Property, s::Property>(m, "Int64Property")
        .def(py::init<>())
        .def_readwrite("Value", &s::Int64Property::Value);

    py::class_<s::IntProperty, s::Property>(m, "IntProperty")
        .def(py::init<>())
        .def_readwrite("Value", &s::IntProperty::Value);

    py::class_<s::MapProperty, s::Property>(m, "MapProperty")
        //.def(py::init<s::PropertyTag>()) // TODO
        .def("keyType", &s::MapProperty::keyType)
        .def("valueType", &s::MapProperty::valueType)
        //.def("keys", &s::MapProperty::keys) // TODO
        //.def("values", &s::MapProperty::values) // TODO
        ;

    py::class_<s::MulticastSparseDelegateProperty, s::Property>(m, "MulticastSparseDelegateProperty")
        .def(py::init<s::PropertyTag>())
        // TODO
        ;

    py::class_<s::NameProperty, s::Property>(m, "NameProperty")
        .def(py::init<>())
        .def_readwrite("Value", &s::NameProperty::Value);

    py::class_<s::ObjectProperty, s::Property>(m, "ObjectProperty")
        .def(py::init<>())
        .def_readwrite("Value", &s::ObjectProperty::Value);

    py::class_<s::SetProperty, s::Property>(m, "SetProperty")
        //.def(py::init<s::PropertyTag>()) // TODO
        // TODO
        ;

    py::class_<s::SoftObjectProperty, s::Property>(m, "SoftObjectProperty")
        .def(py::init<s::PropertyTag>())
        // TODO
        ;

    py::class_<s::StrProperty, s::Property>(m, "StrProperty")
        .def(py::init<>())
        .def_readwrite("Value", &s::StrProperty::Value);

    py::class_<s::StructProperty, s::Property>(m, "StructProperty")
        .def(py::init<s::PropertyTag>())
        // TODO
        ;

    py::class_<s::TextProperty, s::Property>(m, "TextProperty")
        .def(py::init<>())
        .def_readwrite("Value", &s::TextProperty::Value);

    py::class_<s::UInt32Property, s::Property>(m, "UInt32Property")
        .def(py::init<>())
        .def_readwrite("Value", &s::UInt32Property::Value);

    py::class_<s::UInt64Property, s::Property>(m, "UInt64Property")
        .def(py::init<>())
        .def_readwrite("Value", &s::UInt64Property::Value);

    py::class_<s::UnknownProperty, s::Property>(m, "UnknownProperty")
        .def(py::init<s::PropertyTag>())
        .def("value", &s::UnknownProperty::value);
}
