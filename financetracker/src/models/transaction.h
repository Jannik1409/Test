#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>

/**
 * @class Transaction
 * @brief Represents a financial transaction (income or expense)
 */
class Transaction {
public:
    enum class Type {
        Income,
        Expense
    };

    Transaction();
    Transaction(int id, int accountId, const std::string& date, double amount,
                const std::string& description, int categoryId, Type type,
                const std::string& tags = "");
    ~Transaction() = default;

    int getId() const;
    int getAccountId() const;
    std::string getDate() const;
    double getAmount() const;
    std::string getDescription() const;
    int getCategoryId() const;
    Type getType() const;
    std::string getTags() const;
    int getRecurringId() const;
    std::string getCreatedAt() const;
    std::string getUpdatedAt() const;
    bool isDeleted() const;

    void setId(int id);
    void setAccountId(int accountId);
    void setDate(const std::string& date);
    void setAmount(double amount);
    void setDescription(const std::string& description);
    void setCategoryId(int categoryId);
    void setType(Type type);
    void setTags(const std::string& tags);
    void setRecurringId(int id);
    void setCreatedAt(const std::string& createdAt);
    void setUpdatedAt(const std::string& updatedAt);
    void setDeleted(bool deleted);

    std::string getTypeString() const;
    static Type typeFromString(const std::string& str);
    static std::string typeToString(Type type);
    std::vector<std::string> getTagList() const;

private:
    int m_id;
    int m_accountId;
    std::string m_date;
    double m_amount;
    std::string m_description;
    int m_categoryId;
    Type m_type;
    std::string m_tags;
    int m_recurringId;
    std::string m_createdAt;
    std::string m_updatedAt;
    bool m_deleted;
};

#endif // TRANSACTION_H