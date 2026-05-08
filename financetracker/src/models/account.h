#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

/**
 * @class Account
 * @brief Represents a financial account (checking, savings, credit card, etc.)
 */
class Account {
public:
    enum class Type {
        Checking,
        Savings,
        CreditCard,
        Cash,
        Investment
    };

    Account();
    Account(int id, const std::string& name, Type type, double balance, const std::string& color);
    Account(const std::string& name, const std::string& typeStr, double balance, const std::string& color);
    ~Account() = default;

    // Get functions
    int getId() const;
    std::string getName() const;
    Type getType() const;
    double getBalance() const;
    std::string getColor() const;
    std::string getCreatedAt() const;

    // Set functions
    void setId(int id);
    void setName(const std::string& name);
    void setType(Type type);
    void setBalance(double balance);
    void setColor(const std::string& color);
    void setCreatedAt(const std::string& createdAt);

    // Utility functions
    std::string getTypeString() const;
    static Type typeFromString(const std::string& str);
    static std::string typeToString(Type type);

private:
    int m_id;
    std::string m_name;
    Type m_type;
    double m_balance;
    std::string m_color;
    std::string m_createdAt;
};

#endif // ACCOUNT_H