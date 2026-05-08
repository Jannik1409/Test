#include "statistics.h"
#include "../utils/dateutils.h"
#include <algorithm>

double Statistics::calculateTotalIncome(const std::vector<Transaction>& transactions) {
    double total = 0.0;
    for (const auto& t : transactions) {
        if (t.getType() == Transaction::Type::Income) {
            total += t.getAmount();
        }
    }
    return total;
}

double Statistics::calculateTotalExpenses(const std::vector<Transaction>& transactions) {
    double total = 0.0;
    for (const auto& t : transactions) {
        if (t.getType() == Transaction::Type::Expense) {
            total += t.getAmount();
        }
    }
    return total;
}

double Statistics::calculateBalance(const std::vector<Transaction>& transactions) {
    return calculateTotalIncome(transactions) - calculateTotalExpenses(transactions);
}

std::vector<Statistics::MonthlyData> Statistics::getMonthlyData(const std::vector<Transaction>& transactions) {
    std::map<int, MonthlyData> monthlyMap;

    for (const auto& t : transactions) {
        int year = DateUtils::getYearFromDate(t.getDate());
        int month = DateUtils::getMonthFromDate(t.getDate());
        int key = year * 100 + month;

        if (monthlyMap.find(key) == monthlyMap.end()) {
            monthlyMap[key] = {year, month, 0.0, 0.0, 0.0};
        }

        if (t.getType() == Transaction::Type::Income) {
            monthlyMap[key].income += t.getAmount();
        } else {
            monthlyMap[key].expenses += t.getAmount();
        }
    }

    std::vector<MonthlyData> result;
    for (auto& pair : monthlyMap) {
        pair.second.balance = pair.second.income - pair.second.expenses;
        result.push_back(pair.second);
    }

    std::sort(result.begin(), result.end(), [](const MonthlyData& a, const MonthlyData& b) {
        return (a.year * 100 + a.month) < (b.year * 100 + b.month);
    });

    return result;
}

std::vector<Statistics::CategoryData> Statistics::getSpendingByCategory(const std::vector<Transaction>& transactions) {
    std::map<int, double> categoryTotals;
    std::map<int, std::string> categoryNames;

    for (const auto& t : transactions) {
        if (t.getType() == Transaction::Type::Expense && t.getCategoryId() > 0) {
            categoryTotals[t.getCategoryId()] += t.getAmount();
        }
    }

    std::vector<CategoryData> result;
    for (const auto& pair : categoryTotals) {
        result.push_back({pair.first, "", pair.second, ""});
    }

    return result;
}

std::vector<Statistics::CategoryData> Statistics::getIncomeByCategory(const std::vector<Transaction>& transactions) {
    std::map<int, double> categoryTotals;

    for (const auto& t : transactions) {
        if (t.getType() == Transaction::Type::Income && t.getCategoryId() > 0) {
            categoryTotals[t.getCategoryId()] += t.getAmount();
        }
    }

    std::vector<CategoryData> result;
    for (const auto& pair : categoryTotals) {
        result.push_back({pair.first, "", pair.second, ""});
    }

    return result;
}

double Statistics::getAverageExpense(const std::vector<Transaction>& transactions) {
    double total = calculateTotalExpenses(transactions);
    int count = 0;
    for (const auto& t : transactions) {
        if (t.getType() == Transaction::Type::Expense) count++;
    }
    return count > 0 ? total / count : 0.0;
}

double Statistics::getAverageIncome(const std::vector<Transaction>& transactions) {
    double total = calculateTotalIncome(transactions);
    int count = 0;
    for (const auto& t : transactions) {
        if (t.getType() == Transaction::Type::Income) count++;
    }
    return count > 0 ? total / count : 0.0;
}

int Statistics::getTransactionCount(const std::vector<Transaction>& transactions) {
    return static_cast<int>(transactions.size());
}

double Statistics::getCategorySpending(int categoryId, const std::vector<Transaction>& transactions) {
    double total = 0.0;
    for (const auto& t : transactions) {
        if (t.getType() == Transaction::Type::Expense && t.getCategoryId() == categoryId) {
            total += t.getAmount();
        }
    }
    return total;
}

bool Statistics::isOverBudget(const std::vector<Transaction>& transactions, const Category& category) {
    if (category.getBudgetLimit() <= 0) return false;
    double spent = getCategorySpending(category.getId(), transactions);
    return spent > category.getBudgetLimit();
}