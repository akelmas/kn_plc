#include "logger.h"

#include "spdlog/sinks/qt_sinks.h"

void Logger::initialize(QTextEdit* logView,
    int maxLines)
{
    auto logger = spdlog::qt_color_logger_mt("qt_logger", logView, maxLines);
    spdlog::set_default_logger(logger);
    LOGI("Logger initialized.");
}
