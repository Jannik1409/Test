#ifndef SAVINGS_GOAL_H
#define SAVINGS_GOAL_H

#include <string>

/**
 * @class SavingsGoal
 * @brief Represents a savings target with progress tracking
 */
class SavingsGoal {
public:
    SavingsGoal();
    SavingsGoal(int id, const std::string& name, double targetAmount, double currentAmount,
                const std::string& targetDate, const std::string& color);
    ~SavingsGoal() = default;

    int getId() const;
    std::string getName() const;
    double getTargetAmount() const;
    double getCurrentAmount() const;
    std::string getTargetDate() const;
    std::string getColor() const;
    std::string getCreatedAt() const;

    void setId(int id);
    void setName(const std::string& name);
    void setTargetAmount(double amount);
    void setCurrentAmount(double amount);
    void setTargetDate(const std::string& date);
    void setColor(const std::string& color);
    void setCreatedAt(const std::string& date);

    double getProgressPercent() const;
    double getRemainingAmount() const;
    int getDaysRemaining() const;
    bool isCompleted() const;

private:
    int m_id;
    std::string m_name;
    double m_targetAmount;
    double m_currentAmount;
    std::string m_targetDate;
    std::string m_color;
    std::string m_createdAt;
};

#endif // SAVINGS_GOAL_H