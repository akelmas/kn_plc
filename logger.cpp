#include "logger.h"

#include "spdlog/sinks/qt_sinks.h"
#include "spdlog/spdlog.h"

void Logger::initialize(QTextEdit* logView,
    int maxLines)
{
    auto logger = spdlog::qt_color_logger_mt("qt_logger", logView, maxLines);
    spdlog::set_default_logger(logger);
    spdlog::info("new logger log message");
}
