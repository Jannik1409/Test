#include "dateutils.h"
#include "../services/theme_manager.h"
#include <algorithm>
#include <chrono>

int DateUtils::getDaysInMonth(const std::string& date) {
    int month = getMonthFromDate(date);  // use your existing function

    // Extract year from date string for leap year check
    int year = 0;
    if (date.size() >= 4) {
        year = std::stoi(date.substr(0,4));
    }

    if (month == 2) {
        // Leap year calculation
        return ( (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0) ) ? 29 : 28;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    }
    else {
        return 31;
    }
}

std::string DateUtils::currentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string DateUtils::currentDate() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d");
    return ss.str();
}

std::string DateUtils::formatDate(const std::string& date) {
    int year, month, day;
    if (!parseDate(date, year, month, day)) return date;
    
    std::string format = ThemeManager::getDateFormat();
    
    std::stringstream ss;
    if (format == "DD/MM/YYYY") {
        ss << std::setw(2) << std::setfill('0') << day << '/'
           << std::setw(2) << std::setfill('0') << month << '/'
           << year;
    } else if (format == "MM/DD/YYYY") {
        ss << std::setw(2) << std::setfill('0') << month << '/'
           << std::setw(2) << std::setfill('0') << day << '/'
           << year;
    } else {
        ss << year << '-'
           << std::setw(2) << std::setfill('0') << month << '-'
           << std::setw(2) << std::setfill('0') << day;
    }
    return ss.str();
}

std::string DateUtils::getMonthName(int month) {
    static const std::string months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    if (month < 1 || month > 12) return "";
    return months[month - 1];
}

int DateUtils::getMonthFromDate(const std::string& date) {
    int year, month, day;
    if (parseDate(date, year, month, day)) {
        return month;
    }
    return 0;
}

int DateUtils::getYearFromDate(const std::string& date) {
    int year, month, day;
    if (parseDate(date, year, month, day)) {
        return year;
    }
    return 0;
}

std::string DateUtils::addDays(const std::string& date, int days) {
    int year, month, day;
    if (!parseDate(date, year, month, day)) return date;

    tm t = {};
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = 12;
    mktime(&t);
    t.tm_mday += days;
    mktime(&t);

    return makeDate(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
}

std::string DateUtils::addMonths(const std::string& date, int months) {
    int year, month, day;
    if (!parseDate(date, year, month, day)) return date;

    year += (month - 1 + months) / 12;
    month = (month - 1 + months) % 12 + 1;
    day = std::min(day, 28);

    return makeDate(year, month, day);
}

bool DateUtils::isValidDate(const std::string& date) {
    int year, month, day;
    return parseDate(date, year, month, day);
}

std::string DateUtils::getFirstDayOfMonth(const std::string& date) {
    int year, month, day;
    if (!parseDate(date, year, month, day)) return date;
    return makeDate(year, month, 1);
}

std::string DateUtils::getLastDayOfMonth(const std::string& date) {
    int year, month, day;
    if (!parseDate(date, year, month, day)) return date;

    int lastDay = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11) lastDay = 30;
    else if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) lastDay = 29;
        else lastDay = 28;
    }
    return makeDate(year, month, lastDay);
}

int DateUtils::daysBetween(const std::string& date1, const std::string& date2) {
    int y1, m1, d1, y2, m2, d2;
    if (!parseDate(date1, y1, m1, d1) || !parseDate(date2, y2, m2, d2)) return 0;

    tm t1 = {}, t2 = {};
    t1.tm_year = y1 - 1900; t1.tm_mon = m1 - 1; t1.tm_mday = d1;
    t2.tm_year = y2 - 1900; t2.tm_mon = m2 - 1; t2.tm_mday = d2;

    time_t time1 = mktime(&t1);
    time_t time2 = mktime(&t2);
    return static_cast<int>((time2 - time1) / (24 * 60 * 60));
}

bool DateUtils::parseDate(const std::string& date, int& year, int& month, int& day) {
    if (date.length() != 10 || date[4] != '-' || date[7] != '-') return false;
    try {
        year = std::stoi(date.substr(0, 4));
        month = std::stoi(date.substr(5, 2));
        day = std::stoi(date.substr(8, 2));
        return year > 0 && month >= 1 && month <= 12 && day >= 1 && day <= 31;
    } catch (...) {
        return false;
    }
}

std::string DateUtils::makeDate(int year, int month, int day) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << year << '-'
       << std::setw(2) << month << '-' << std::setw(2) << day;
    return ss.str();
}
