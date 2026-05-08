#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

/**
 * @class Logger
 * @brief Simple logging utility for debug/info/error messages
 */
class Logger {
public:
    enum class Level {
        Debug,
        Info,
        Warning,
        Error
    };

    static void init(const std::string& logFile = "finance_tracker.log");
    static void setLevel(Level level);
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);

private:
    static std::string levelToString(Level level);
    static void log(Level level, const std::string& message);

    static Level s_level;
    static std::ofstream s_file;
    static std::mutex s_mutex;
    static bool s_initialized;
};

#endif // LOGGER_H