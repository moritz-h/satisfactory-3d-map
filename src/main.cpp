#include <filesystem>
#include <iostream>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "MapWindow/MapWindow.h"
#include "Utils/ConsoleUtils.h"

int main(int argc, char* argv[]) {
    try {
        Satisfactory3DMap::setConsoleUtf8();
        auto cout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        cout_sink->set_pattern("[%T.%e] %^[%L] %v%$");
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Satisfactory3DMap.log", true);
        file_sink->set_pattern("[%Y-%m-%d %T.%e %z] [%-8l] %v");
        auto logger =
            std::make_shared<spdlog::logger>("", std::initializer_list<spdlog::sink_ptr>{cout_sink, file_sink});
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);
        spdlog::set_default_logger(logger);
    } catch (const std::exception& ex) {
        std::cout << "Failed to initialize logger: " << ex.what() << std::endl;
        return 1;
    }

    try {
        spdlog::info("[Satisfactory3DMap]");
        if (argc > 2) {
            spdlog::warn("Usage: Satisfactory3DMap [filename]");
            return 1;
        }
        std::filesystem::path savefile;
        if (argc == 2) {
            savefile = std::filesystem::path(argv[1]);
        }

        Satisfactory3DMap::MapWindow window;
        window.dataView()->openSave(savefile);
        window.run();
    } catch (const std::exception& ex) {
        spdlog::error("Exception: {}", ex.what());
        return 1;
    } catch (...) {
        spdlog::error("Exception: Unknown error!");
        return 1;
    }

    return 0;
}
