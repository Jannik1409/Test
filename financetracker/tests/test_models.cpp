#include <gtest/gtest.h>
#include "../src/models/transaction.h"

TEST(TransactionTest, DefaultValues) {
    Transaction t;
    EXPECT_EQ(t.getAmount(), 0.0);
    EXPECT_EQ(t.getDescription(), "");
}

TEST(TransactionTest, SettersGetters) {
    Transaction t;
    t.setAmount(100.0);
    t.setDescription("Test");
    EXPECT_EQ(t.getAmount(), 100.0);
    EXPECT_EQ(t.getDescription(), "Test");
}