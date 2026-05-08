#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <string>
#include <vector>
#include "../models/transaction.h"
#include "../models/category.h"
#include "../models/account.h"
#include "../models/savings_goal.h"
#include "../models/tag.h"

/**
 * @class JsonHandler
 * @brief Handle JSON import/export for data portability
 */
class JsonHandler {
public:
    static bool exportToFile(const std::string& filePath,
                             const std::vector<Transaction>& transactions,
                             const std::vector<Category>& categories,
                             const std::vector<Account>& accounts,
                             const std::vector<SavingsGoal>& goals,
                             const std::vector<Tag>& tags);

    static bool importFromFile(const std::string& filePath,
                               std::vector<Transaction>& transactions,
                               std::vector<Category>& categories,
                               std::vector<Account>& accounts,
                               std::vector<SavingsGoal>& goals,
                               std::vector<Tag>& tags);

    static std::string toJson(const std::vector<Transaction>& transactions);
    static std::string transactionsToJson(const std::vector<Transaction>& transactions);
    static std::string categoriesToJson(const std::vector<Category>& categories);
    static std::string accountsToJson(const std::vector<Account>& accounts);
};

#endif // JSON_HANDLER_H