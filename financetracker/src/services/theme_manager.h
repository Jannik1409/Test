#ifndef THEME_MANAGER_H
#define THEME_MANAGER_H

#include <string>

/**
 * @class ThemeManager
 * @brief Manage application themes (dark/light mode)
 */
class ThemeManager {
public:
    enum class Theme {
        Light,
        Dark,
        System
    };

    static Theme getCurrentTheme();
    static void setTheme(Theme theme);
    static bool isDarkMode();
    static void loadSettings();
    static void saveSettings();

    // Color constants
    static std::string primaryColor();
    static std::string secondaryColor();
    static std::string backgroundColor();
    static std::string textColor();
    static std::string incomeColor();
    static std::string expenseColor();
    static std::string getDateFormat();
    static void setDateFormat(const std::string& format);

private:
    static Theme s_currentTheme;
    static std::string s_settingsFile;
    static std::string s_dateFormat;
    static std::string getSettingsFile();
};

#endif // THEME_MANAGER_H