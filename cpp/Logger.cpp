#include "headers/Logger.h"

Logger::Logger() {
    if (!std::experimental::filesystem::exists("./logs")) {
        std::experimental::filesystem::create_directory("./logs");
    }
    logFile_ = "./logs/" + getTimestamp() + ".log";
}

void Logger::log(Level level, const std::string& message) {
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

void Logger::setConsoleLevel(Level level) {
    consoleLevel_ = level;
}

void Logger::info(const std::string& msg) {
    getInstance().log(Level::INFO, msg);
}

void Logger::debug(const std::string& msg) {
    getInstance().log(Level::DEBUG, msg);
}

void Logger::error(const std::string& msg) {
    getInstance().log(Level::ERROR, msg);
}

void Logger::critical(const std::string& msg) {
    getInstance().log(Level::ERROR, msg);
}

void Logger::warning(const std::string& message) {
    getInstance().log(Level::WARNING, message);
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y.%m.%d_%H:%M:%S");
    return ss.str();
}

std::string Logger::levelToString(Level level) {
    switch (level) {
        case Level::DEBUG: return "DEBUG";
        case Level::INFO: return "INFO";
        case Level::WARNING: return "WARNING";
        case Level::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}