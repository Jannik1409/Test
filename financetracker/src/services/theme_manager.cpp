#include "theme_manager.h"
#include "../utils/logger.h"
#include "../../include/nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

ThemeManager::Theme ThemeManager::s_currentTheme = Theme::Light;
std::string ThemeManager::s_settingsFile = "data/settings.json";
std::string ThemeManager::s_dateFormat = "YYYY-MM-DD";

ThemeManager::Theme ThemeManager::getCurrentTheme() {
    return s_currentTheme;
}

void ThemeManager::setTheme(Theme theme) {
    s_currentTheme = theme;
    saveSettings();
    Logger::info("Theme changed to: " + std::to_string(static_cast<int>(theme)));
}

bool ThemeManager::isDarkMode() {
    if (s_currentTheme == Theme::System) {
        return false;
    }
    return s_currentTheme == Theme::Dark;
}

void ThemeManager::loadSettings() {
    std::ifstream file(s_settingsFile);
    if (file.is_open()) {
        try {
            json j;
            file >> j;
            
            if (j.contains("theme")) {
                std::string themeStr = j["theme"];
                if (themeStr == "dark") s_currentTheme = Theme::Dark;
                else if (themeStr == "system") s_currentTheme = Theme::System;
                else s_currentTheme = Theme::Light;
            }
            
            if (j.contains("dateFormat")) {
                s_dateFormat = j["dateFormat"];
            }
        } catch (...) {
            s_currentTheme = Theme::Light;
            s_dateFormat = "YYYY-MM-DD";
        }
        file.close();
    }
}

void ThemeManager::saveSettings() {
    json j;
    
    std::ifstream inputFile(s_settingsFile);
    if (inputFile.is_open()) {
        try {
            inputFile >> j;
        } catch (...) {
            j = json::object();
        }
        inputFile.close();
    }
    
    if (s_currentTheme == Theme::Dark) j["theme"] = "dark";
    else if (s_currentTheme == Theme::System) j["theme"] = "system";
    else j["theme"] = "light";
    
    j["dateFormat"] = s_dateFormat;
    
    std::ofstream outputFile(s_settingsFile);
    if (outputFile.is_open()) {
        outputFile << j.dump(4);
        outputFile.close();
    }
}

std::string ThemeManager::primaryColor() {
    return isDarkMode() ? "#1ABC9C" : "#2C3E50";
}

std::string ThemeManager::secondaryColor() {
    return isDarkMode() ? "#34495E" : "#3498DB";
}

std::string ThemeManager::backgroundColor() {
    return isDarkMode() ? "#2C3E50" : "#ECF0F1";
}

std::string ThemeManager::textColor() {
    return isDarkMode() ? "#ECF0F1" : "#2C3E50";
}

std::string ThemeManager::incomeColor() {
    return "#27AE60";
}

std::string ThemeManager::expenseColor() {
    return "#E74C3C";
}

std::string ThemeManager::getDateFormat() {
    return s_dateFormat;
}

void ThemeManager::setDateFormat(const std::string& format) {
    s_dateFormat = format;
    saveSettings();
}

std::string ThemeManager::getSettingsFile() {
    return s_settingsFile;
}