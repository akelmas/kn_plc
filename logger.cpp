#include "logger.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/sinks/qt_sinks.h"
#include "spdlog/spdlog.h"

#define LOGT SPDLOG_TRACE
#define LOGD SPDLOG_DEBUG
#define LOGI SPDLOG_INFO
#define LOGW SPDLOG_WARN
#define LOGE SPDLOG_ERROR
#define LOGC SPDLOG_CRITICAL

void Logger::initialize(QTextEdit* logView,
    int maxLines)
{
    auto logger = spdlog::qt_color_logger_mt("qt_logger", logView, maxLines);
    spdlog::set_default_logger(logger);
    LOGI("Logger initialized.");
}
