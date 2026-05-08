#include <gtest/gtest.h>
#include "../src/services/statistics.h"
#include "../src/models/transaction.h"

TEST(StatisticsTest, CalculateTotalIncome) {
    Statistics stats;
    std::vector<Transaction> transactions;
    
    Transaction t1;
    t1.setType(Transaction::Type::Income);
    t1.setAmount(1000.0);
    transactions.push_back(t1);
    
    Transaction t2;
    t2.setType(Transaction::Type::Expense);
    t2.setAmount(500.0);
    transactions.push_back(t2);
    
    EXPECT_EQ(stats.calculateTotalIncome(transactions), 1000.0);
}

TEST(StatisticsTest, CalculateTotalExpenses) {
    Statistics stats;
    std::vector<Transaction> transactions;
    
    Transaction t1;
    t1.setType(Transaction::Type::Income);
    t1.setAmount(1000.0);
    transactions.push_back(t1);
    
    Transaction t2;
    t2.setType(Transaction::Type::Expense);
    t2.setAmount(500.0);
    transactions.push_back(t2);
    
    EXPECT_EQ(stats.calculateTotalExpenses(transactions), 500.0);
}