#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include "SatisfactorySave/GameTypes/Save/SaveActor.h"
#include "SatisfactorySave/GameTypes/Save/SaveGame.h"
#include "SatisfactorySave/GameTypes/Save/SaveObject.h"
#include "SatisfactorySave/GameTypes/Save/SaveObjectBase.h"
#include "libsavepy_common.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_GameTypes_Save(py::module_& m) {
    py::class_<s::SaveObjectBase, std::shared_ptr<s::SaveObjectBase>>(m, "SaveObjectBase")
        .def_readwrite("ClassName", &s::SaveObjectBase::ClassName)
        .def_readwrite("Reference", &s::SaveObjectBase::Reference)
        .def_readonly("Properties", &s::SaveObjectBase::Properties) // TODO write
        .def_readwrite("HasGuid", &s::SaveObjectBase::HasGuid)
        .def_readwrite("Guid", &s::SaveObjectBase::Guid)
        .def_readwrite("ExtraProperties", &s::SaveObjectBase::ExtraProperties)
        .def("isActor", &s::SaveObjectBase::isActor);

    py::class_<s::SaveObject, s::SaveObjectBase, std::shared_ptr<s::SaveObject>>(m, "SaveObject")
        .def(py::init<>())
        .def_readwrite("OuterPathName", &s::SaveObject::OuterPathName);

    py::class_<s::SaveActor, s::SaveObjectBase, std::shared_ptr<s::SaveActor>>(m, "SaveActor")
        .def(py::init<>())
        .def_readwrite("Transform", &s::SaveActor::Transform)
        .def_readwrite("NeedTransform", &s::SaveActor::NeedTransform)
        .def_readwrite("WasPlacedInLevel", &s::SaveActor::WasPlacedInLevel)
        .def("parentReference", &s::SaveActor::parentReference)
        .def("childReferences", &s::SaveActor::childReferences)
        .def_readwrite("unk1", &s::SaveActor::unk1)
        .def_readwrite("unk2", &s::SaveActor::unk2);

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
        .def("getObjectsByPath", &s::SaveGame::getObjectsByPath)
        .def("levelRootNodes", &s::SaveGame::levelRootNodes)
        .def("persistentAndRuntimeRootNode", &s::SaveGame::persistentAndRuntimeRootNode)
        .def("allRootNode", &s::SaveGame::allRootNode)
        .def("getGlobalId", &s::SaveGame::getGlobalId);
}
