#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include "SatisfactorySave/GameTypes/Save/SaveActor.h"
#include "SatisfactorySave/GameTypes/Save/SaveGame.h"
#include "SatisfactorySave/GameTypes/Save/SaveObject.h"
#include "SatisfactorySave/GameTypes/Save/SaveObjectBase.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_GameTypes_Save(py::module_& m) {
    py::class_<s::SaveObjectBase, std::shared_ptr<s::SaveObjectBase>>(m, "SaveObjectBase")
        .def_readwrite("ClassName", &s::SaveObjectBase::ClassName)
        .def_readwrite("Reference", &s::SaveObjectBase::Reference)
        //.def_readwrite("Properties", &s::SaveObjectBase::Properties) // TODO
        .def_readwrite("HasGuid", &s::SaveObjectBase::HasGuid)
        .def_readwrite("Guid", &s::SaveObjectBase::Guid)
        .def_readwrite("ExtraProperties", &s::SaveObjectBase::ExtraProperties)
        .def("globalId", &s::SaveObjectBase::globalId)
        .def("levelId", &s::SaveObjectBase::levelId)
        .def("type", &s::SaveObjectBase::type);

    py::class_<s::SaveObject, s::SaveObjectBase, std::shared_ptr<s::SaveObject>>(m, "SaveObject")
        .def_readwrite("OuterPathName", &s::SaveObject::OuterPathName);

    py::class_<s::SaveActor, s::SaveObjectBase, std::shared_ptr<s::SaveActor>>(m, "SaveActor")
        .def_readwrite("Transform", &s::SaveActor::Transform)
        .def_readwrite("NeedTransform", &s::SaveActor::NeedTransform)
        .def_readwrite("WasPlacedInLevel", &s::SaveActor::WasPlacedInLevel)
        .def("parentReference", &s::SaveActor::parentReference)
        .def("childReferences", &s::SaveActor::childReferences);

    py::class_<s::SaveGame>(m, "SaveGame")
        .def(py::init<const std::filesystem::path&>())
        .def("save", &s::SaveGame::save)
        .def("header", &s::SaveGame::header)
        .def("levelData", &s::SaveGame::levelData)
        .def("saveObjects", &s::SaveGame::saveObjects)
        .def("destroyedActors", &s::SaveGame::destroyedActors)
        .def("unresolvedWorldSaveData", &s::SaveGame::unresolvedWorldSaveData)
        .def("allSaveObjects", &s::SaveGame::allSaveObjects)
        .def("getObjectsByPath", &s::SaveGame::getObjectsByPath)
        .def("levelRootNodes", &s::SaveGame::levelRootNodes)
        .def("rootNode", &s::SaveGame::rootNode)
        .def("allRootNode", &s::SaveGame::allRootNode);
}
