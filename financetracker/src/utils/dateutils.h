#ifndef DATEUTILS_H
#define DATEUTILS_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

class ThemeManager;

/**
 * @class DateUtils
 * @brief Utility class for date and time operations
 */
class DateUtils {
public:
    static int getDaysInMonth(const std::string& date);
    static std::string currentDateTime();
    static std::string currentDate();
    static std::string formatDate(const std::string& date);
    static std::string getMonthName(int month);
    static int getMonthFromDate(const std::string& date);
    static int getYearFromDate(const std::string& date);
    static std::string addDays(const std::string& date, int days);
    static std::string addMonths(const std::string& date, int months);
    static bool isValidDate(const std::string& date);
    static std::string getFirstDayOfMonth(const std::string& date);
    static std::string getLastDayOfMonth(const std::string& date);
    static int daysBetween(const std::string& date1, const std::string& date2);

private:
    static bool parseDate(const std::string& date, int& year, int& month, int& day);
    static std::string makeDate(int year, int month, int day);
};

#endif // DATEUTILS_H
