#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

/**
 * @class Validator
 * @brief Data validation utilities
 */
class Validator {
public:
    static bool isValidAmount(double amount);
    static bool isValidDate(const std::string& date);
    static bool isValidDescription(const std::string& desc);
    static bool isValidName(const std::string& name);
    static bool isValidColor(const std::string& color);
    static std::string validateTransaction(double amount, const std::string& desc, const std::string& date);
    static std::string validateAccount(const std::string& name);
    static std::string validateCategory(const std::string& name);

private:
    static bool isValidHexColor(const std::string& color);
};

#endif // VALIDATOR_H