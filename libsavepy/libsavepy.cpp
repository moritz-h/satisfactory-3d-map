#include <pybind11/pybind11.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "SatisfactorySave/GameTypes/Save/SaveGame.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_GameTypes_Arrays(py::module_&);
void init_GameTypes_MapTypes(py::module_&);
void init_GameTypes_Properties(py::module_&);
void init_GameTypes_Save(py::module_&);
void init_GameTypes_Sets(py::module_&);
void init_GameTypes_Structs(py::module_&);
void init_GameTypes_UE_Internationalization(py::module_&);
void init_GameTypes_UE_Math(py::module_&);
void init_GameTypes_UE_Misc(py::module_&);
void init_GameTypes_UE_Satisfactory(py::module_&);
void init_GameTypes_UE_UObject(py::module_&);

PYBIND11_MODULE(satisfactory_save, m) {
    // Register a spdlog default logger
    auto cout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    cout_sink->set_pattern("[%T.%e] %^[%L] %v%$");
    auto logger = std::make_shared<spdlog::logger>("", std::initializer_list<spdlog::sink_ptr>{cout_sink});
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);
    spdlog::set_default_logger(logger);

    init_GameTypes_Arrays(m);
    init_GameTypes_MapTypes(m);
    init_GameTypes_Properties(m);
    init_GameTypes_Save(m);
    init_GameTypes_Sets(m);
    init_GameTypes_Structs(m);
    init_GameTypes_UE_Internationalization(m);
    init_GameTypes_UE_Math(m);
    init_GameTypes_UE_Misc(m);
    init_GameTypes_UE_Satisfactory(m);
    init_GameTypes_UE_UObject(m);
}
