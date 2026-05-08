#ifndef ADDTRANSACTIONDIALOG_H
#define ADDTRANSACTIONDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QDateEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>

#include "../../database/database.h"
#include "../../models/transaction.h"

/**
 * @class AddTransactionDialog
 * @brief Dialog for adding or editing transactions
 */
class AddTransactionDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddTransactionDialog(Database& db, QWidget* parent = nullptr);
    explicit AddTransactionDialog(Database& db, const Transaction* transaction, QWidget* parent = nullptr);
    ~AddTransactionDialog() = default;

private slots:
    void onSave();
    void onCancel();
    void onTypeChanged();

private:
    void setupUi();
    void loadCategories();

    Database& m_db;
    const Transaction* m_editTransaction;
    bool m_isEditing;

    QDateEdit* m_dateEdit;
    QLineEdit* m_amountEdit;
    QLineEdit* m_descriptionEdit;
    QComboBox* m_categoryCombo;
    QRadioButton* m_incomeRadio;
    QRadioButton* m_expenseRadio;
    QComboBox* m_accountCombo;
    QLineEdit* m_tagsEdit;
    QPushButton* m_saveButton;
    QPushButton* m_cancelButton;
};

#endif // ADDTRANSACTIONDIALOG_H