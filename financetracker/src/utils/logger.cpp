#include "logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

bool Logger::s_initialized = false;
Logger::Level Logger::s_level = Level::Info;
std::ofstream Logger::s_file;
std::mutex Logger::s_mutex;

void Logger::init(const std::string& logFile) {
    if (s_initialized) return;
    s_file.open(logFile, std::ios::app);
    s_initialized = true;
    info("Logger initialized");
}

void Logger::setLevel(Level level) {
    s_level = level;
}

void Logger::debug(const std::string& message) {
    log(Level::Debug, message);
}

void Logger::info(const std::string& message) {
    log(Level::Info, message);
}

void Logger::warning(const std::string& message) {
    log(Level::Warning, message);
}

void Logger::error(const std::string& message) {
    log(Level::Error, message);
}

std::string Logger::levelToString(Level level) {
    switch (level) {
        case Level::Debug: return "DEBUG";
        case Level::Info: return "INFO";
        case Level::Warning: return "WARN";
        case Level::Error: return "ERROR";
        default: return "UNKNOWN";
    }
}

void Logger::log(Level level, const std::string& message) {
    if (!s_initialized || level < s_level) return;

    std::lock_guard<std::mutex> lock(s_mutex);
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    std::string timestamp = ss.str();

    std::string logLine = "[" + timestamp + "] [" + levelToString(level) + "] " + message;

    if (s_file.is_open()) {
        s_file << logLine << std::endl;
        s_file.flush();
    }
    std::cout << logLine << std::endl;
}
