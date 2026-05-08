#include "recurring.h"
#include "../utils/dateutils.h"

Recurring::Recurring()
    : m_id(0), m_categoryId(0), m_amount(0.0), m_description(""),
      m_frequency(Frequency::Monthly), m_startDate(DateUtils::currentDate()),
      m_endDate(""), m_lastGenerated("") {}

Recurring::Recurring(int id, int categoryId, double amount, const std::string& description,
                     Frequency frequency, const std::string& startDate, const std::string& endDate)
    : m_id(id), m_categoryId(categoryId), m_amount(amount), m_description(description),
      m_frequency(frequency), m_startDate(startDate), m_endDate(endDate), m_lastGenerated("") {}

int Recurring::getId() const { return m_id; }
int Recurring::getCategoryId() const { return m_categoryId; }
double Recurring::getAmount() const { return m_amount; }
std::string Recurring::getDescription() const { return m_description; }
Recurring::Frequency Recurring::getFrequency() const { return m_frequency; }
std::string Recurring::getStartDate() const { return m_startDate; }
std::string Recurring::getEndDate() const { return m_endDate; }
std::string Recurring::getLastGenerated() const { return m_lastGenerated; }
bool Recurring::isActive() const {
    std::string today = DateUtils::currentDate();
    if (!m_endDate.empty() && today > m_endDate) return false;
    return true;
}

void Recurring::setId(int id) { m_id = id; }
void Recurring::setCategoryId(int id) { m_categoryId = id; }
void Recurring::setAmount(double amount) { m_amount = amount; }
void Recurring::setDescription(const std::string& desc) { m_description = desc; }
void Recurring::setFrequency(Frequency freq) { m_frequency = freq; }
void Recurring::setStartDate(const std::string& date) { m_startDate = date; }
void Recurring::setEndDate(const std::string& date) { m_endDate = date; }
void Recurring::setLastGenerated(const std::string& date) { m_lastGenerated = date; }

std::string Recurring::getFrequencyString() const { return frequencyToString(m_frequency); }

Recurring::Frequency Recurring::frequencyFromString(const std::string& str) {
    if (str == "Daily") return Frequency::Daily;
    if (str == "Weekly") return Frequency::Weekly;
    if (str == "Biweekly") return Frequency::Biweekly;
    if (str == "Monthly") return Frequency::Monthly;
    if (str == "Yearly") return Frequency::Yearly;
    return Frequency::Monthly;
}

std::string Recurring::frequencyToString(Frequency freq) {
    switch (freq) {
        case Frequency::Daily: return "Daily";
        case Frequency::Weekly: return "Weekly";
        case Frequency::Biweekly: return "Biweekly";
        case Frequency::Monthly: return "Monthly";
        case Frequency::Yearly: return "Yearly";
        default: return "Monthly";
    }
}

int Recurring::getNextIntervalDays() const {
    switch (m_frequency) {
        case Frequency::Daily: return 1;
        case Frequency::Weekly: return 7;
        case Frequency::Biweekly: return 14;
        case Frequency::Monthly: return 30;
        case Frequency::Yearly: return 365;
        default: return 30;
    }
}