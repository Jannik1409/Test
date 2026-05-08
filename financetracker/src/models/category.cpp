#include "category.h"
#include "../utils/dateutils.h"

Category::Category()
    : m_id(0), m_name(""), m_type(Type::Expense), m_color("#3498DB"), m_budgetLimit(0.0) {
    m_createdAt = DateUtils::currentDateTime();
}

Category::Category(int id, const std::string& name, Type type, const std::string& color, double budgetLimit)
    : m_id(id), m_name(name), m_type(type), m_color(color), m_budgetLimit(budgetLimit) {
    m_createdAt = DateUtils::currentDateTime();
}

Category::Category(const std::string& name, Type type, const std::string& color, double budgetLimit)
    : m_id(0), m_name(name), m_type(type), m_color(color), m_budgetLimit(budgetLimit) {
    m_createdAt = DateUtils::currentDateTime();
}

int Category::getId() const { return m_id; }
std::string Category::getName() const { return m_name; }
Category::Type Category::getType() const { return m_type; }
std::string Category::getColor() const { return m_color; }
double Category::getBudgetLimit() const { return m_budgetLimit; }
std::string Category::getCreatedAt() const { return m_createdAt; }

void Category::setId(int id) { m_id = id; }
void Category::setName(const std::string& name) { m_name = name; }
void Category::setType(Type type) { m_type = type; }
void Category::setColor(const std::string& color) { m_color = color; }
void Category::setBudgetLimit(double limit) { m_budgetLimit = limit; }
void Category::setCreatedAt(const std::string& createdAt) { m_createdAt = createdAt; }

std::string Category::getTypeString() const {
    return typeToString(m_type);
}

Category::Type Category::typeFromString(const std::string& str) {
    return (str == "Income") ? Type::Income : Type::Expense;
}

std::string Category::typeToString(Type type) {
    return (type == Type::Income) ? "Income" : "Expense";
}