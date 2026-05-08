#include "validator.h"
#include "dateutils.h"
#include <regex>

bool Validator::isValidAmount(double amount) {
    return amount > 0 && amount < 1000000000;
}

bool Validator::isValidDate(const std::string& date) {
    return DateUtils::isValidDate(date);
}

bool Validator::isValidDescription(const std::string& desc) {
    return !desc.empty() && desc.length() <= 500;
}

bool Validator::isValidName(const std::string& name) {
    return !name.empty() && name.length() <= 100;
}

bool Validator::isValidColor(const std::string& color) {
    return isValidHexColor(color);
}

bool Validator::isValidHexColor(const std::string& color) {
    std::regex pattern("^#[0-9A-Fa-f]{6}$");
    return std::regex_match(color, pattern);
}

std::string Validator::validateTransaction(double amount, const std::string& desc, const std::string& date) {
    if (!isValidAmount(amount)) {
        return "Invalid amount. Must be positive and less than 1 billion.";
    }
    if (!isValidDescription(desc)) {
        return "Invalid description. Must be 1-500 characters.";
    }
    if (!isValidDate(date)) {
        return "Invalid date format. Use YYYY-MM-DD.";
    }
    return "";
}

std::string Validator::validateAccount(const std::string& name) {
    if (!isValidName(name)) {
        return "Invalid account name. Must be 1-100 characters.";
    }
    return "";
}

std::string Validator::validateCategory(const std::string& name) {
    if (!isValidName(name)) {
        return "Invalid category name. Must be 1-100 characters.";
    }
    return "";
}