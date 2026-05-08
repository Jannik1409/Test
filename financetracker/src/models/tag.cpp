#include "tag.h"
#include "../utils/dateutils.h"

Tag::Tag()
    : m_id(0), m_name(""), m_color("#9B59B6") {
    m_createdAt = DateUtils::currentDateTime();
}

Tag::Tag(int id, const std::string& name, const std::string& color)
    : m_id(id), m_name(name), m_color(color) {
    m_createdAt = DateUtils::currentDateTime();
}

int Tag::getId() const { return m_id; }
std::string Tag::getName() const { return m_name; }
std::string Tag::getColor() const { return m_color; }
std::string Tag::getCreatedAt() const { return m_createdAt; }

void Tag::setId(int id) { m_id = id; }
void Tag::setName(const std::string& name) { m_name = name; }
void Tag::setColor(const std::string& color) { m_color = color; }
void Tag::setCreatedAt(const std::string& date) { m_createdAt = date; }