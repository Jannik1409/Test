#include "categorydialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QListWidget>

CategoryDialog::CategoryDialog(Database& db, QWidget* parent)
    : QDialog(parent), m_db(db), m_selectedId(0) {
    setWindowTitle("Manage Categories");
    resize(600, 400);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    
    m_categoryList = new QListWidget(this);
    mainLayout->addWidget(m_categoryList);
    
    QVBoxLayout* rightLayout = new QVBoxLayout();
    
    QHBoxLayout* nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel("Name:"));
    m_nameEdit = new QLineEdit(this);
    nameLayout->addWidget(m_nameEdit);
    rightLayout->addLayout(nameLayout);
    
    QHBoxLayout* typeLayout = new QHBoxLayout();
    typeLayout->addWidget(new QLabel("Type:"));
    m_typeCombo = new QComboBox(this);
    m_typeCombo->addItem("Expense", "Expense");
    m_typeCombo->addItem("Income", "Income");
    typeLayout->addWidget(m_typeCombo);
    rightLayout->addLayout(typeLayout);
    
    QHBoxLayout* colorLayout = new QHBoxLayout();
    colorLayout->addWidget(new QLabel("Color:"));
    m_colorEdit = new QLineEdit(this);
    m_colorEdit->setPlaceholderText("#3498DB");
    colorLayout->addWidget(m_colorEdit);
    rightLayout->addLayout(colorLayout);
    
    QHBoxLayout* budgetLayout = new QHBoxLayout();
    budgetLayout->addWidget(new QLabel("Budget:"));
    m_budgetEdit = new QLineEdit(this);
    m_budgetEdit->setPlaceholderText("0.00");
    budgetLayout->addWidget(m_budgetEdit);
    rightLayout->addLayout(budgetLayout);
    
    m_addButton = new QPushButton("Add");
    m_editButton = new QPushButton("Edit");
    m_deleteButton = new QPushButton("Delete");
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_editButton);
    buttonLayout->addWidget(m_deleteButton);
    rightLayout->addLayout(buttonLayout);
    
    rightLayout->addStretch();
    
    m_saveButton = new QPushButton("Close");
    rightLayout->addWidget(m_saveButton);
    
    mainLayout->addLayout(rightLayout);
    
    loadCategories();
    
    connect(m_addButton, &QPushButton::clicked, this, &CategoryDialog::onAdd);
    connect(m_editButton, &QPushButton::clicked, this, &CategoryDialog::onEdit);
    connect(m_deleteButton, &QPushButton::clicked, this, &CategoryDialog::onDelete);
    connect(m_saveButton, &QPushButton::clicked, this, &CategoryDialog::accept);
    connect(m_categoryList, &QListWidget::itemSelectionChanged, this, &CategoryDialog::onSelectionChanged);
}

void CategoryDialog::loadCategories() {
    m_categories = m_db.getAllCategories();
    m_categoryList->clear();
    
    for (const auto& cat : m_categories) {
        QString text = QString::fromStdString(cat.getName()) + " (" + QString::fromStdString(cat.getTypeString()) + ")";
        QListWidgetItem* item = new QListWidgetItem(text, m_categoryList);
        item->setData(Qt::UserRole, cat.getId());
    }
    
    updateButtons();
}

void CategoryDialog::updateButtons() {
    bool hasSelection = m_categoryList->currentRow() >= 0;
    m_editButton->setEnabled(hasSelection);
    m_deleteButton->setEnabled(hasSelection);
}

void CategoryDialog::onAdd() {
    QString name = m_nameEdit->text();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a category name.");
        return;
    }
    
    Category cat;
    cat.setName(name.toStdString());
    cat.setType(Category::typeFromString(m_typeCombo->currentText().toStdString()));
    cat.setColor(m_colorEdit->text().toStdString());
    cat.setBudgetLimit(m_budgetEdit->text().toDouble());
    
    int id = m_db.createCategory(cat);
    if (id > 0) {
        loadCategories();
        m_nameEdit->clear();
    } else {
        QMessageBox::critical(this, "Error", "Failed to create category.");
    }
}

void CategoryDialog::onEdit() {
    int row = m_categoryList->currentRow();
    if (row < 0) return;
    
    int id = m_categoryList->currentItem()->data(Qt::UserRole).toInt();
    
    for (const auto& cat : m_categories) {
        if (cat.getId() == id) {
            Category updatedCat = cat;
            updatedCat.setName(m_nameEdit->text().toStdString());
            updatedCat.setType(Category::typeFromString(m_typeCombo->currentText().toStdString()));
            updatedCat.setColor(m_colorEdit->text().toStdString());
            updatedCat.setBudgetLimit(m_budgetEdit->text().toDouble());
            m_db.updateCategory(updatedCat);
            loadCategories();
            break;
        }
    }
}

void CategoryDialog::onDelete() {
    int row = m_categoryList->currentRow();
    if (row < 0) return;
    
    if (QMessageBox::question(this, "Confirm", "Delete this category?") == QMessageBox::Yes) {
        int id = m_categoryList->currentItem()->data(Qt::UserRole).toInt();
        m_db.deleteCategory(id);
        loadCategories();
    }
}

void CategoryDialog::onSelectionChanged() {
    int row = m_categoryList->currentRow();
    if (row < 0) {
        updateButtons();
        return;
    }
    
    int id = m_categoryList->currentItem()->data(Qt::UserRole).toInt();
    
    for (const auto& cat : m_categories) {
        if (cat.getId() == id) {
            m_nameEdit->setText(QString::fromStdString(cat.getName()));
            m_typeCombo->setCurrentText(QString::fromStdString(cat.getTypeString()));
            m_colorEdit->setText(QString::fromStdString(cat.getColor()));
            m_budgetEdit->setText(QString::number(cat.getBudgetLimit(), 'f', 2));
            break;
        }
    }
    
    updateButtons();
}