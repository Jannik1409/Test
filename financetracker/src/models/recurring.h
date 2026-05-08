#ifndef RECURRING_H
#define RECURRING_H

#include <string>

/**
 * @class Recurring
 * @brief Represents a recurring transaction template
 */
class Recurring {
public:
    enum class Frequency {
        Daily,
        Weekly,
        Biweekly,
        Monthly,
        Yearly
    };

    Recurring();
    Recurring(int id, int categoryId, double amount, const std::string& description,
              Frequency frequency, const std::string& startDate, const std::string& endDate);
    ~Recurring() = default;

    int getId() const;
    int getCategoryId() const;
    double getAmount() const;
    std::string getDescription() const;
    Frequency getFrequency() const;
    std::string getStartDate() const;
    std::string getEndDate() const;
    std::string getLastGenerated() const;
    bool isActive() const;

    void setId(int id);
    void setCategoryId(int id);
    void setAmount(double amount);
    void setDescription(const std::string& desc);
    void setFrequency(Frequency freq);
    void setStartDate(const std::string& date);
    void setEndDate(const std::string& date);
    void setLastGenerated(const std::string& date);

    std::string getFrequencyString() const;
    static Frequency frequencyFromString(const std::string& str);
    static std::string frequencyToString(Frequency freq);
    int getNextIntervalDays() const;

private:
    int m_id;
    int m_categoryId;
    double m_amount;
    std::string m_description;
    Frequency m_frequency;
    std::string m_startDate;
    std::string m_endDate;
    std::string m_lastGenerated;
};

#endif // RECURRING_H