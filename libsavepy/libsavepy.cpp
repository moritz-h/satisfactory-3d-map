#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "libsavepy_common.h"

PYBIND11_MODULE(satisfactory_save, m) {
    // Register a spdlog default logger
    auto cout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    cout_sink->set_pattern("[%T.%e] %^[%L] %v%$");
    auto logger = std::make_shared<spdlog::logger>("", std::initializer_list<spdlog::sink_ptr>{cout_sink});
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);
    spdlog::set_default_logger(logger);

    init_all(m);
}
