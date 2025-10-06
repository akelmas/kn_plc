#ifndef LOGGER_H
#define LOGGER_H

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "item.h"

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#define LOGT SPDLOG_TRACE
#define LOGD SPDLOG_DEBUG
#define LOGI SPDLOG_INFO
#define LOGW SPDLOG_WARN
#define LOGE SPDLOG_ERROR
#define LOGC SPDLOG_CRITICAL

template <>
struct fmt::formatter<Item> : ostream_formatter { };

class QTextEdit;
class Logger {
public:
    static void initialize(QTextEdit* logView, int maxLines);

private:
    Logger() = delete;
};

inline std::ostream& operator<<(std::ostream& os, const Item& item)
{
    return os << fmt::format("Item<{},{}>", item.pos().first, item.pos().second);
}

#endif // LOGGER_H
