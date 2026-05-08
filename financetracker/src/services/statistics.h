#ifndef STATISTICS_H
#define STATISTICS_H

#include <string>
#include <vector>
#include <map>
#include "../models/transaction.h"
#include "../models/category.h"

/**
 * @class Statistics
 * @brief Calculate financial statistics and reports
 */
class Statistics {
public:
    struct MonthlyData {
        int year;
        int month;
        double income;
        double expenses;
        double balance;
    };

    struct CategoryData {
        int categoryId;
        std::string categoryName;
        double total;
        std::string color;
    };

    struct DateRange {
        std::string start;
        std::string end;
    };

    Statistics() = default;

    double calculateTotalIncome(const std::vector<Transaction>& transactions);
    double calculateTotalExpenses(const std::vector<Transaction>& transactions);
    double calculateBalance(const std::vector<Transaction>& transactions);
    std::vector<MonthlyData> getMonthlyData(const std::vector<Transaction>& transactions);
    std::vector<CategoryData> getSpendingByCategory(const std::vector<Transaction>& transactions);
    std::vector<CategoryData> getIncomeByCategory(const std::vector<Transaction>& transactions);
    double getAverageExpense(const std::vector<Transaction>& transactions);
    double getAverageIncome(const std::vector<Transaction>& transactions);
    int getTransactionCount(const std::vector<Transaction>& transactions);
    double getCategorySpending(int categoryId, const std::vector<Transaction>& transactions);
    bool isOverBudget(const std::vector<Transaction>& transactions, const Category& category);

private:
    std::map<int, double> groupByMonth(const std::vector<Transaction>& transactions);
    std::map<int, double> groupByCategory(const std::vector<Transaction>& transactions, Transaction::Type type);
};

#endif // STATISTICS_H