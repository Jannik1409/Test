#include "json_handler.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

bool JsonHandler::exportToFile(const std::string& filePath,
                                const std::vector<Transaction>& transactions,
                                const std::vector<Category>& categories,
                                const std::vector<Account>& accounts,
                                const std::vector<SavingsGoal>& goals,
                                const std::vector<Tag>& tags) {
    nlohmann::json j;
    j["version"] = "1.0";
    j["export_date"] = nlohmann::json::object({{"date", ""}}); // Placeholder

    for (const auto& acc : accounts) {
        j["accounts"].push_back({
            {"id", acc.getId()},
            {"name", acc.getName()},
            {"type", acc.getTypeString()},
            {"balance", acc.getBalance()},
            {"color", acc.getColor()}
        });
    }

    for (const auto& cat : categories) {
        j["categories"].push_back({
            {"id", cat.getId()},
            {"name", cat.getName()},
            {"type", cat.getTypeString()},
            {"color", cat.getColor()},
            {"budget_limit", cat.getBudgetLimit()}
        });
    }

    for (const auto& t : transactions) {
        j["transactions"].push_back({
            {"id", t.getId()},
            {"account_id", t.getAccountId()},
            {"date", t.getDate()},
            {"amount", t.getAmount()},
            {"description", t.getDescription()},
            {"category_id", t.getCategoryId()},
            {"type", t.getTypeString()},
            {"tags", t.getTags()}
        });
    }

    for (const auto& g : goals) {
        j["savings_goals"].push_back({
            {"id", g.getId()},
            {"name", g.getName()},
            {"target_amount", g.getTargetAmount()},
            {"current_amount", g.getCurrentAmount()},
            {"target_date", g.getTargetDate()},
            {"color", g.getColor()}
        });
    }

    for (const auto& tag : tags) {
        j["tags"].push_back({
            {"id", tag.getId()},
            {"name", tag.getName()},
            {"color", tag.getColor()}
        });
    }

    std::ofstream file(filePath);
    if (!file.is_open()) return false;
    file << j.dump(4);
    file.close();
    return true;
}

bool JsonHandler::importFromFile(const std::string& filePath,
                                 std::vector<Transaction>& transactions,
                                 std::vector<Category>& categories,
                                 std::vector<Account>& accounts,
                                 std::vector<SavingsGoal>& goals,
                                 std::vector<Tag>& tags) {
    std::ifstream file(filePath);
    if (!file.is_open()) return false;

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    try {
        nlohmann::json j = nlohmann::json::parse(content);

        accounts.clear();
        if (j.contains("accounts")) {
            for (const auto& acc : j["accounts"]) {
                Account a;
                a.setId(acc.value("id", 0));
                a.setName(acc.value("name", ""));
                a.setType(Account::typeFromString(acc.value("type", "Checking")));
                a.setBalance(acc.value("balance", 0.0));
                a.setColor(acc.value("color", "#3498DB"));
                accounts.push_back(a);
            }
        }

        categories.clear();
        if (j.contains("categories")) {
            for (const auto& cat : j["categories"]) {
                Category c;
                c.setId(cat.value("id", 0));
                c.setName(cat.value("name", ""));
                c.setType(Category::typeFromString(cat.value("type", "Expense")));
                c.setColor(cat.value("color", "#3498DB"));
                c.setBudgetLimit(cat.value("budget_limit", 0.0));
                categories.push_back(c);
            }
        }

        transactions.clear();
        if (j.contains("transactions")) {
            for (const auto& t : j["transactions"]) {
                Transaction trans;
                trans.setId(t.value("id", 0));
                trans.setAccountId(t.value("account_id", 0));
                trans.setDate(t.value("date", ""));
                trans.setAmount(t.value("amount", 0.0));
                trans.setDescription(t.value("description", ""));
                trans.setCategoryId(t.value("category_id", 0));
                trans.setType(Transaction::typeFromString(t.value("type", "Expense")));
                trans.setTags(t.value("tags", ""));
                transactions.push_back(trans);
            }
        }

        goals.clear();
        if (j.contains("savings_goals")) {
            for (const auto& g : j["savings_goals"]) {
                SavingsGoal goal;
                goal.setId(g.value("id", 0));
                goal.setName(g.value("name", ""));
                goal.setTargetAmount(g.value("target_amount", 0.0));
                goal.setCurrentAmount(g.value("current_amount", 0.0));
                goal.setTargetDate(g.value("target_date", ""));
                goal.setColor(g.value("color", "#27AE60"));
                goals.push_back(goal);
            }
        }

        tags.clear();
        if (j.contains("tags")) {
            for (const auto& tag : j["tags"]) {
                Tag t;
                t.setId(tag.value("id", 0));
                t.setName(tag.value("name", ""));
                t.setColor(tag.value("color", "#9B59B6"));
                tags.push_back(t);
            }
        }

        return true;
    } catch (...) {
        return false;
    }
}

std::string JsonHandler::toJson(const std::vector<Transaction>& transactions) {
    nlohmann::json j;
    for (const auto& t : transactions) {
        j["transactions"].push_back({
            {"id", t.getId()},
            {"account_id", t.getAccountId()},
            {"date", t.getDate()},
            {"amount", t.getAmount()},
            {"description", t.getDescription()},
            {"category_id", t.getCategoryId()},
            {"type", t.getTypeString()},
            {"tags", t.getTags()}
        });
    }
    return j.dump();
}

std::string JsonHandler::transactionsToJson(const std::vector<Transaction>& transactions) {
    return toJson(transactions);
}

std::string JsonHandler::categoriesToJson(const std::vector<Category>& categories) {
    nlohmann::json j;
    for (const auto& c : categories) {
        j["categories"].push_back({
            {"id", c.getId()},
            {"name", c.getName()},
            {"type", c.getTypeString()},
            {"color", c.getColor()},
            {"budget_limit", c.getBudgetLimit()}
        });
    }
    return j.dump();
}

std::string JsonHandler::accountsToJson(const std::vector<Account>& accounts) {
    nlohmann::json j;
    for (const auto& a : accounts) {
        j["accounts"].push_back({
            {"id", a.getId()},
            {"name", a.getName()},
            {"type", a.getTypeString()},
            {"balance", a.getBalance()},
            {"color", a.getColor()}
        });
    }
    return j.dump();
}