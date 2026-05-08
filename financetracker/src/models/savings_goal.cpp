#include "savings_goal.h"
#include "../utils/dateutils.h"

SavingsGoal::SavingsGoal()
    : m_id(0), m_name(""), m_targetAmount(0.0), m_currentAmount(0.0),
      m_targetDate(""), m_color("#27AE60") {
    m_createdAt = DateUtils::currentDateTime();
}

SavingsGoal::SavingsGoal(int id, const std::string& name, double targetAmount, double currentAmount,
                         const std::string& targetDate, const std::string& color)
    : m_id(id), m_name(name), m_targetAmount(targetAmount), m_currentAmount(currentAmount),
      m_targetDate(targetDate), m_color(color) {
    m_createdAt = DateUtils::currentDateTime();
}

int SavingsGoal::getId() const { return m_id; }
std::string SavingsGoal::getName() const { return m_name; }
double SavingsGoal::getTargetAmount() const { return m_targetAmount; }
double SavingsGoal::getCurrentAmount() const { return m_currentAmount; }
std::string SavingsGoal::getTargetDate() const { return m_targetDate; }
std::string SavingsGoal::getColor() const { return m_color; }
std::string SavingsGoal::getCreatedAt() const { return m_createdAt; }

void SavingsGoal::setId(int id) { m_id = id; }
void SavingsGoal::setName(const std::string& name) { m_name = name; }
void SavingsGoal::setTargetAmount(double amount) { m_targetAmount = amount; }
void SavingsGoal::setCurrentAmount(double amount) { m_currentAmount = amount; }
void SavingsGoal::setTargetDate(const std::string& date) { m_targetDate = date; }
void SavingsGoal::setColor(const std::string& color) { m_color = color; }
void SavingsGoal::setCreatedAt(const std::string& date) { m_createdAt = date; }

double SavingsGoal::getProgressPercent() const {
    if (m_targetAmount <= 0) return 0.0;
    return std::min(100.0, (m_currentAmount / m_targetAmount) * 100.0);
}

double SavingsGoal::getRemainingAmount() const {
    return std::max(0.0, m_targetAmount - m_currentAmount);
}

int SavingsGoal::getDaysRemaining() const {
    if (m_targetDate.empty()) return -1;
    return DateUtils::daysBetween(DateUtils::currentDate(), m_targetDate);
}

bool SavingsGoal::isCompleted() const {
    return m_currentAmount >= m_targetAmount;
}