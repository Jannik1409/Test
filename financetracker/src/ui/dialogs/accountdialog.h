#ifndef ACCOUNTDIALOG_H
#define ACCOUNTDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>

#include "../../database/database.h"
#include "../../models/account.h"

/**
 * @class AccountDialog
 * @brief Dialog for managing accounts
 */
class AccountDialog : public QDialog {
    Q_OBJECT

public:
    explicit AccountDialog(Database& db, QWidget* parent = nullptr);
    ~AccountDialog() = default;

private slots:
    void onAdd();

private:
    void loadAccounts();

    Database& m_db;
    std::vector<Account> m_accounts;

    QListWidget* m_accountList;
    QLineEdit* m_nameEdit;
    QComboBox* m_typeCombo;
    QLineEdit* m_balanceEdit;
    QLineEdit* m_colorEdit;
    QPushButton* m_saveButton;
    QPushButton* m_cancelButton;
};

#endif // ACCOUNTDIALOG_H