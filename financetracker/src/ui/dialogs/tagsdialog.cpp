#include "tagsdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

TagsDialog::TagsDialog(Database& db, QWidget* parent)
    : QDialog(parent), m_db(db) {
    setWindowTitle("Manage Tags");
    resize(400, 300);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    m_list = new QListWidget(this);
    mainLayout->addWidget(m_list);
    
    QHBoxLayout* inputLayout = new QHBoxLayout();
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("New tag name...");
    inputLayout->addWidget(m_nameEdit);
    m_addButton = new QPushButton("Add", this);
    inputLayout->addWidget(m_addButton);
    mainLayout->addLayout(inputLayout);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_deleteButton = new QPushButton("Delete", this);
    m_closeButton = new QPushButton("Close", this);
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_closeButton);
    mainLayout->addLayout(buttonLayout);
    
    loadTags();
    
    connect(m_addButton, &QPushButton::clicked, this, &TagsDialog::onAdd);
    connect(m_deleteButton, &QPushButton::clicked, this, &TagsDialog::onDelete);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
}

void TagsDialog::loadTags() {
    m_tags = m_db.getAllTags();
    m_list->clear();
    for (const auto& tag : m_tags) {
        m_list->addItem(QString::fromStdString(tag.getName()));
    }
}

void TagsDialog::onAdd() {
    QString name = m_nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a tag name.");
        return;
    }
    
    Tag tag;
    tag.setName(name.toStdString());
    
    int id = m_db.createTag(tag);
    if (id > 0) {
        loadTags();
        m_nameEdit->clear();
    } else {
        QMessageBox::critical(this, "Error", "Failed to create tag. Tag may already exist.");
    }
}

void TagsDialog::onDelete() {
    int row = m_list->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a tag to delete.");
        return;
    }
    
    if (QMessageBox::question(this, "Confirm", "Delete this tag?") == QMessageBox::Yes) {
        int id = m_tags[row].getId();
        m_db.deleteTag(id);
        loadTags();
    }
}