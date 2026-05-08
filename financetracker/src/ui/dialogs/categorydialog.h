#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>

#include "../../database/database.h"
#include "../../models/category.h"

/**
 * @class CategoryDialog
 * @brief Dialog for managing transaction categories
 */
class CategoryDialog : public QDialog {
    Q_OBJECT

public:
    explicit CategoryDialog(Database& db, QWidget* parent = nullptr);
    ~CategoryDialog() = default;

private slots:
    void onAdd();
    void onEdit();
    void onDelete();
    void onSelectionChanged();

private:
    void loadCategories();
    void updateButtons();

    Database& m_db;
    std::vector<Category> m_categories;
    int m_selectedId;

    QListWidget* m_categoryList;
    QLineEdit* m_nameEdit;
    QComboBox* m_typeCombo;
    QLineEdit* m_colorEdit;
    QLineEdit* m_budgetEdit;
    QPushButton* m_addButton;
    QPushButton* m_editButton;
    QPushButton* m_deleteButton;
    QPushButton* m_saveButton;
    QPushButton* m_cancelButton;
};

#endif // CATEGORYDIALOG_H