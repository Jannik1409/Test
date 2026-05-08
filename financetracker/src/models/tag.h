#ifndef TAG_H
#define TAG_H

#include <string>

/**
 * @class Tag
 * @brief Represents a tag for categorizing transactions
 */
class Tag {
public:
    Tag();
    Tag(int id, const std::string& name, const std::string& color);
    ~Tag() = default;

    int getId() const;
    std::string getName() const;
    std::string getColor() const;
    std::string getCreatedAt() const;

    void setId(int id);
    void setName(const std::string& name);
    void setColor(const std::string& color);
    void setCreatedAt(const std::string& date);

private:
    int m_id;
    std::string m_name;
    std::string m_color;
    std::string m_createdAt;
};

#endif // TAG_H