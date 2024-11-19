#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <memory>
#include <mutex>

class Logger {
public:
    enum class Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(Level level, const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::string levelStr = levelToString(level);
        std::string timestamp = getTimestamp();

        std::ofstream file(logFile_, std::ios_base::app);
        if (file.is_open()) {
            file << timestamp << " - " << levelStr << " - " << message << std::endl;
        }

        if (level >= consoleLevel_) {
            std::cout << timestamp << " - " << levelStr << " - " << message << std::endl;
        }
    }

    void setConsoleLevel(Level level) {
        consoleLevel_ = level;
    }

private:
    Logger() {
        if (!std::filesystem::exists("./logs")) {
            std::filesystem::create_directory("./logs");
        }
        logFile_ = "./logs/" + getTimestamp() + ".log";
    }

    std::string getTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y.%m.%d_%H:%M:%S");
        return ss.str();
    }

    std::string levelToString(Level level) {
        switch (level) {
            case Level::DEBUG: return "DEBUG";
            case Level::INFO: return "INFO";
            case Level::WARNING: return "WARNING";
            case Level::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }

    std::string logFile_;
    Level consoleLevel_ = Level::INFO;
    std::mutex mutex_;
};