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

    py::class_<s::SaveGame::PerLevelData>(m, "PerLevelData")
        // Readonly type
        .def_readwrite("level_name", &s::SaveGame::PerLevelData::level_name)
        .def_readwrite("save_objects", &s::SaveGame::PerLevelData::save_objects)
        .def_readwrite("destroyed_actors_TOC", &s::SaveGame::PerLevelData::destroyed_actors_TOC)
        .def_readwrite("destroyed_actors", &s::SaveGame::PerLevelData::destroyed_actors);

    py::class_<s::SaveGame::PersistentAndRuntimeData>(m, "PersistentAndRuntimeData")
        // Readonly type
        .def_readwrite("save_objects", &s::SaveGame::PersistentAndRuntimeData::save_objects)
        .def_readwrite("destroyed_actors", &s::SaveGame::PersistentAndRuntimeData::destroyed_actors_TOC);

    py::class_<s::SaveGame::SaveNode>(m, "SaveNode")
        // Readonly type
        .def_readwrite("childNodes", &s::SaveGame::SaveNode::childNodes)
        .def_readwrite("objects", &s::SaveGame::SaveNode::objects)
        .def_readwrite("numObjects", &s::SaveGame::SaveNode::numObjects)
        .def_readwrite("numActors", &s::SaveGame::SaveNode::numActors);

    py::class_<s::SaveGame>(m, "SaveGame")
        .def(py::init<const std::filesystem::path&>())
        .def("save", &s::SaveGame::save)
        .def("header", &s::SaveGame::header)
        .def("validationData", &s::SaveGame::validationData)
        .def("perLevelData", &s::SaveGame::perLevelData)
        .def("persistentAndRuntimeData", &s::SaveGame::persistentAndRuntimeData)
        .def("unresolvedWorldSaveData", &s::SaveGame::unresolvedWorldSaveData)
        .def("allSaveObjects", &s::SaveGame::allSaveObjects)
        .def("isObjectPath", &s::SaveGame::isObjectPath)
        .def("getObjectsByPath", &s::SaveGame::getObjectsByPath)
        .def("levelRootNodes", &s::SaveGame::levelRootNodes)
        .def("persistentAndRuntimeRootNode", &s::SaveGame::persistentAndRuntimeRootNode)
        .def("allRootNode", &s::SaveGame::allRootNode)
        .def("getGlobalId", &s::SaveGame::getGlobalId)
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
