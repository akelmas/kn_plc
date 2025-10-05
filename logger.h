#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>

class QTextEdit;
class Logger {
public:
    static void initialize(QTextEdit* logView, int maxLines);

private:
    Logger() = delete;
};

#endif // LOGGER_H
