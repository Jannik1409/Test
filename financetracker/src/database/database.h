#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <memory>
#include <sqlite3.h>

#include "../models/account.h"
#include "../models/category.h"
#include "../models/transaction.h"
#include "../models/recurring.h"
#include "../models/savings_goal.h"
#include "../models/tag.h"

/**
 * @class Database
 * @brief SQLite database wrapper for persistent storage
 */
class Database {
public:
    explicit Database(const std::string& dbPath = "data/finance.db");
    ~Database();

    bool open();
    void close();
    bool isOpen() const;

    // Account operations
    int createAccount(const Account& account);
    bool updateAccount(const Account& account);
    bool deleteAccount(int id);
    std::vector<Account> getAllAccounts() const;
    Account getAccountById(int id) const;

    // Category operations
    int createCategory(const Category& category);
    bool updateCategory(const Category& category);
    bool deleteCategory(int id);
    std::vector<Category> getAllCategories() const;
    Category getCategoryById(int id) const;
    std::vector<Category> getCategoriesByType(Category::Type type) const;

    // Transaction operations
    int createTransaction(const Transaction& trans);
    bool updateTransaction(const Transaction& trans);
    bool deleteTransaction(int id);
    std::vector<Transaction> getAllTransactions() const;
    std::vector<Transaction> getTransactionsByAccount(int accountId) const;
    std::vector<Transaction> getTransactionsByDateRange(const std::string& start, const std::string& end) const;
    std::vector<Transaction> searchTransactions(const std::string& query) const;
    Transaction getTransactionById(int id) const;

    // Recurring operations
    int createRecurring(const Recurring& recurring);
    bool updateRecurring(const Recurring& recurring);
    bool deleteRecurring(int id);
    std::vector<Recurring> getAllRecurrings() const;
    std::vector<Recurring> getActiveRecurrings() const;

    // Savings Goal operations
    int createSavingsGoal(const SavingsGoal& goal);
    bool updateSavingsGoal(const SavingsGoal& goal);
    bool deleteSavingsGoal(int id);
    std::vector<SavingsGoal> getAllSavingsGoals() const;
    SavingsGoal getSavingsGoalById(int id) const;

    // Tag operations
    int createTag(const Tag& tag);
    bool deleteTag(int id);
    std::vector<Tag> getAllTags() const;

    // Utility
    bool backup(const std::string& backupPath) const;
    std::string getLastError() const;

private:
    bool execute(const std::string& sql);
    bool prepareStatement(const std::string& sql);
    void resetStatement();
    bool step();
    std::string getColumnText(int col) const;
    double getColumnDouble(int col) const;
    int getColumnInt(int col) const;

    sqlite3* m_db;
    std::string m_dbPath;
    std::string m_lastError;
    sqlite3_stmt* m_stmt;

    // Schema version
    static const int SCHEMA_VERSION = 1;
    bool createSchema();
    bool insertDefaultData();
    void seedDefaultData();
};

#endif // DATABASE_H