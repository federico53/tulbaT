// Logger.h
#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <mutex>

class Logger {
public:
    enum class Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    static Logger& getInstance();

    void log(Level level, const std::string& message);
    void setConsoleLevel(Level level);
    static void info(const std::string& msg);
    static void debug(const std::string& msg);
    static void error(const std::string& msg);
    static void critical(const std::string& msg);
    static void warning(const std::string& message);

private:
    Logger();
    std::string getTimestamp();
    std::string levelToString(Level level);

    std::string logFile_;
    Level consoleLevel_ = Level::INFO;
    std::mutex mutex_;
};

#endif // LOGGER_H