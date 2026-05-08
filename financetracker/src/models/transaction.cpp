#include "transaction.h"
#include "../utils/dateutils.h"
#include <sstream>
#include <algorithm>

Transaction::Transaction()
    : m_id(0), m_accountId(0), m_date(""), m_amount(0.0), m_description(""),
      m_categoryId(0), m_type(Type::Expense), m_tags(""), m_recurringId(0),
      m_createdAt(DateUtils::currentDateTime()), m_updatedAt(""), m_deleted(false) {}

Transaction::Transaction(int id, int accountId, const std::string& date, double amount,
                         const std::string& description, int categoryId, Type type, const std::string& tags)
    : m_id(id), m_accountId(accountId), m_date(date), m_amount(amount),
      m_description(description), m_categoryId(categoryId), m_type(type),
      m_tags(tags), m_recurringId(0), m_createdAt(DateUtils::currentDateTime()),
      m_updatedAt(""), m_deleted(false) {}

int Transaction::getId() const { return m_id; }
int Transaction::getAccountId() const { return m_accountId; }
std::string Transaction::getDate() const { return m_date; }
double Transaction::getAmount() const { return m_amount; }
std::string Transaction::getDescription() const { return m_description; }
int Transaction::getCategoryId() const { return m_categoryId; }
Transaction::Type Transaction::getType() const { return m_type; }
std::string Transaction::getTags() const { return m_tags; }
int Transaction::getRecurringId() const { return m_recurringId; }
std::string Transaction::getCreatedAt() const { return m_createdAt; }
std::string Transaction::getUpdatedAt() const { return m_updatedAt; }
bool Transaction::isDeleted() const { return m_deleted; }

void Transaction::setId(int id) { m_id = id; }
void Transaction::setAccountId(int id) { m_accountId = id; }
void Transaction::setDate(const std::string& date) { m_date = date; }
void Transaction::setAmount(double amount) { m_amount = amount; }
void Transaction::setDescription(const std::string& desc) { m_description = desc; }
void Transaction::setCategoryId(int id) { m_categoryId = id; }
void Transaction::setType(Type type) { m_type = type; }
void Transaction::setTags(const std::string& tags) { m_tags = tags; }
void Transaction::setRecurringId(int id) { m_recurringId = id; }
void Transaction::setCreatedAt(const std::string& date) { m_createdAt = date; }
void Transaction::setUpdatedAt(const std::string& date) { m_updatedAt = date; }
void Transaction::setDeleted(bool deleted) { m_deleted = deleted; }

std::string Transaction::getTypeString() const {
    return typeToString(m_type);
}

Transaction::Type Transaction::typeFromString(const std::string& str) {
    return (str == "Income") ? Type::Income : Type::Expense;
}

std::string Transaction::typeToString(Type type) {
    return (type == Type::Income) ? "Income" : "Expense";
}

std::vector<std::string> Transaction::getTagList() const {
    std::vector<std::string> result;
    if (m_tags.empty()) return result;
    std::stringstream ss(m_tags);
    std::string tag;
    while (std::getline(ss, tag, ',')) {
        if (!tag.empty()) result.push_back(tag);
    }
    return result;
}