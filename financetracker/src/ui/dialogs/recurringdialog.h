#ifndef RECURRINGDIALOG_H
#define RECURRINGDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QCheckBox>

#include "../../database/database.h"
#include "../../models/recurring.h"

class RecurringDialog : public QDialog {
    Q_OBJECT
public:
    explicit RecurringDialog(Database& db, QWidget* parent = nullptr);
    
private slots:
    void onAdd();
    void onEdit();
    void onDelete();
    void onToggleActive();
    void onSelectionChanged();
    
private:
    void loadRecurrings();
    void updateButtons();
    void loadCategories();
    
    Database& m_db;
    std::vector<Recurring> m_recurrings;
    std::vector<Category> m_categories;
    
    QListWidget* m_list;
    QLineEdit* m_descriptionEdit;
    QLineEdit* m_amountEdit;
    QComboBox* m_categoryCombo;
    QComboBox* m_frequencyCombo;
    QDateEdit* m_startDateEdit;
    QDateEdit* m_endDateEdit;
    QCheckBox* m_activeCheck;
    QPushButton* m_addButton;
    QPushButton* m_editButton;
    QPushButton* m_deleteButton;
    QPushButton* m_closeButton;
};

#endif // RECURRINGDIALOG_H