#include "accountdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>

AccountDialog::AccountDialog(Database& db, QWidget* parent)
    : QDialog(parent), m_db(db) {
    setWindowTitle("Manage Accounts");
    resize(500, 350);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    m_accountList = new QListWidget(this);
    mainLayout->addWidget(m_accountList);
    
    QHBoxLayout* nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel("Name:"));
    m_nameEdit = new QLineEdit(this);
    nameLayout->addWidget(m_nameEdit);
    mainLayout->addLayout(nameLayout);
    
    QHBoxLayout* typeLayout = new QHBoxLayout();
    typeLayout->addWidget(new QLabel("Type:"));
    m_typeCombo = new QComboBox(this);
    m_typeCombo->addItem("Checking");
    m_typeCombo->addItem("Savings");
    m_typeCombo->addItem("CreditCard");
    m_typeCombo->addItem("Cash");
    m_typeCombo->addItem("Investment");
    typeLayout->addWidget(m_typeCombo);
    mainLayout->addLayout(typeLayout);
    
    QHBoxLayout* balanceLayout = new QHBoxLayout();
    balanceLayout->addWidget(new QLabel("Balance:"));
    m_balanceEdit = new QLineEdit(this);
    m_balanceEdit->setText("0.00");
    balanceLayout->addWidget(m_balanceEdit);
    mainLayout->addLayout(balanceLayout);
    
    QHBoxLayout* colorLayout = new QHBoxLayout();
    colorLayout->addWidget(new QLabel("Color:"));
    m_colorEdit = new QLineEdit(this);
    m_colorEdit->setText("#3498DB");
    colorLayout->addWidget(m_colorEdit);
    mainLayout->addLayout(colorLayout);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_saveButton = new QPushButton("Add Account");
    m_cancelButton = new QPushButton("Close");
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_cancelButton);
    mainLayout->addLayout(buttonLayout);
    
    loadAccounts();
    
    connect(m_saveButton, &QPushButton::clicked, this, &AccountDialog::onAdd);
    connect(m_cancelButton, &QPushButton::clicked, this, &AccountDialog::accept);
}

void AccountDialog::loadAccounts() {
    m_accounts = m_db.getAllAccounts();
    m_accountList->clear();
    for (const auto& acc : m_accounts) {
        QString text = QString::fromStdString(acc.getName()) + " - " + QString::number(acc.getBalance(), 'f', 2);
        m_accountList->addItem(text);
    }
}

void AccountDialog::onAdd() {
    QString name = m_nameEdit->text();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter an account name.");
        return;
    }
    
    Account acc;
    acc.setName(name.toStdString());
    acc.setType(Account::typeFromString(m_typeCombo->currentText().toStdString()));
    acc.setBalance(m_balanceEdit->text().toDouble());
    acc.setColor(m_colorEdit->text().toStdString());
    
    int id = m_db.createAccount(acc);
    if (id > 0) {
        loadAccounts();
        m_nameEdit->clear();
    } else {
        QMessageBox::critical(this, "Error", "Failed to create account.");
    }
}