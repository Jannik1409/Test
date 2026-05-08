#include "account.h"
#include "../utils/dateutils.h"

Account::Account()
    : m_id(0), m_name(""), m_type(Type::Checking), m_balance(0.0), m_color("#3498DB") {
    m_createdAt = DateUtils::currentDateTime();
}

Account::Account(int id, const std::string& name, Type type, double balance, const std::string& color)
    : m_id(id), m_name(name), m_type(type), m_balance(balance), m_color(color) {
    m_createdAt = DateUtils::currentDateTime();
}

Account::Account(const std::string& name, const std::string& typeStr, double balance, const std::string& color)
    : m_id(0), m_name(name), m_type(typeFromString(typeStr)), m_balance(balance), m_color(color) {
    m_createdAt = DateUtils::currentDateTime();
}

int Account::getId() const { return m_id; }
std::string Account::getName() const { return m_name; }
Account::Type Account::getType() const { return m_type; }
double Account::getBalance() const { return m_balance; }
std::string Account::getColor() const { return m_color; }
std::string Account::getCreatedAt() const { return m_createdAt; }

void Account::setId(int id) { m_id = id; }
void Account::setName(const std::string& name) { m_name = name; }
void Account::setType(Type type) { m_type = type; }
void Account::setBalance(double balance) { m_balance = balance; }
void Account::setColor(const std::string& color) { m_color = color; }
void Account::setCreatedAt(const std::string& createdAt) { m_createdAt = createdAt; }

std::string Account::getTypeString() const {
    return typeToString(m_type);
}

Account::Type Account::typeFromString(const std::string& str) {
    if (str == "Checking") return Type::Checking;
    if (str == "Savings") return Type::Savings;
    if (str == "CreditCard") return Type::CreditCard;
    if (str == "Cash") return Type::Cash;
    if (str == "Investment") return Type::Investment;
    return Type::Checking;
}

std::string Account::typeToString(Type type) {
    switch (type) {
        case Type::Checking: return "Checking";
        case Type::Savings: return "Savings";
        case Type::CreditCard: return "CreditCard";
        case Type::Cash: return "Cash";
        case Type::Investment: return "Investment";
        default: return "Checking";
    }
}