#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>

/**
 * @class Category
 * @brief Represents a transaction category with optional budget limit
 */
class Category {
public:
    enum class Type {
        Income,
        Expense
    };

    Category();
    Category(int id, const std::string& name, Type type, const std::string& color, double budgetLimit = 0.0);
    Category(const std::string& name, Type type, const std::string& color, double budgetLimit = 0.0);
    ~Category() = default;

    int getId() const;
    std::string getName() const;
    Type getType() const;
    std::string getColor() const;
    double getBudgetLimit() const;
    std::string getCreatedAt() const;

    void setId(int id);
    void setName(const std::string& name);
    void setType(Type type);
    void setColor(const std::string& color);
    void setBudgetLimit(double limit);
    void setCreatedAt(const std::string& createdAt);

    std::string getTypeString() const;
    static Type typeFromString(const std::string& str);
    static std::string typeToString(Type type);

private:
    int m_id;
    std::string m_name;
    Type m_type;
    std::string m_color;
    double m_budgetLimit;
    std::string m_createdAt;
};

#endif // CATEGORY_H