#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include "SatisfactorySave/GameTypes/Save/Blueprint.h"
#include "SatisfactorySave/GameTypes/Save/BlueprintCfg.h"
#include "SatisfactorySave/GameTypes/Save/SaveGame.h"
#include "SatisfactorySave/GameTypes/Save/SaveObject.h"
#include "libsavepy_common.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_GameTypes_Save(py::module_& m) {
    py::class_<s::SaveObject, std::shared_ptr<s::SaveObject>>(m, "SaveObject")
        .def(py::init<>())
        .def_readwrite("Header", &s::SaveObject::Header)
        .def_readwrite("SaveVersion", &s::SaveObject::SaveVersion)
        .def_readwrite("ShouldMigrateObjectRefsToPersistent", &s::SaveObject::ShouldMigrateObjectRefsToPersistent)
        .def_readwrite("Object", &s::SaveObject::Object)
        .def_property("BinaryClassData",
            [](s::SaveObject& o) -> py::bytes {
                return {o.BinaryClassData.data(), o.BinaryClassData.size()};
            },
            [](s::SaveObject& o, const std::string& v) {
                o.BinaryClassData = std::vector<char>(v.begin(), v.end());
            })
        .def_property("BaseHeader",
            [](s::SaveObject& o) -> s::FObjectBaseSaveHeader& {
                return o.baseHeader();
            },
            [](s::SaveObject& o, const s::FObjectBaseSaveHeader& h) {
                o.baseHeader() = h;
            })
        .def("isActor", &s::SaveObject::isActor);

    py::class_<s::SaveGame>(m, "SaveGame")
        .def(py::init<const std::filesystem::path&>())
        .def_readwrite("mSaveHeader", &s::SaveGame::mSaveHeader)
        .def_readwrite("SaveGameValidationData", &s::SaveGame::SaveGameValidationData)
        .def_readwrite("mPerLevelDataMap", &s::SaveGame::mPerLevelDataMap)
        .def_readwrite("mPersistentAndRuntimeData", &s::SaveGame::mPersistentAndRuntimeData)
        .def_readwrite("mUnresolvedWorldSaveData", &s::SaveGame::mUnresolvedWorldSaveData)
        .def("save", &s::SaveGame::save)
        .def("allSaveObjects", &s::SaveGame::allSaveObjects)
        .def("isObjectClass", &s::SaveGame::isObjectClass)
        .def("getObjectsByClass", &s::SaveGame::getObjectsByClass)
        .def("isObjectPath", &s::SaveGame::isObjectPath)
        .def("getObjectsByPath", &s::SaveGame::getObjectsByPath)
        .def("addObject", &s::SaveGame::addObject, py::arg("obj"), py::arg("level") = -1)
        .def("addObjects", &s::SaveGame::addObjects, py::arg("objects"), py::arg("level") = -1)
        .def("removeObject", &s::SaveGame::removeObject, py::arg("obj"))
        .def("removeObjects", &s::SaveGame::removeObjects, py::arg("objects"));

    py::class_<s::BlueprintCfg>(m, "BlueprintCfg")
        .def(py::init<>())
        .def(py::init<const std::filesystem::path&>())
        .def("save", &s::BlueprintCfg::save)
        .def_readwrite("record", &s::BlueprintCfg::record);

    py::class_<s::Blueprint>(m, "Blueprint")
        .def(py::init<>())
        .def(py::init<const std::filesystem::path&>())
        .def("save", &s::Blueprint::save)
        .def_readwrite("header", &s::Blueprint::header)
        .def_readwrite("saveObjects", &s::Blueprint::saveObjects);
}
