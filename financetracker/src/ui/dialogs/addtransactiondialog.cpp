#include "addtransactiondialog.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>

#include "../../services/theme_manager.h"
#include "../../utils/validator.h"

AddTransactionDialog::AddTransactionDialog(Database& db, QWidget* parent)
    : QDialog(parent), m_db(db), m_editTransaction(nullptr), m_isEditing(false) {
    setupUi();
    loadCategories();
}

AddTransactionDialog::AddTransactionDialog(Database& db, const Transaction* transaction, QWidget* parent)
    : QDialog(parent), m_db(db), m_editTransaction(transaction), m_isEditing(true) {
    setupUi();
    loadCategories();
    
    if (m_editTransaction) {
        m_dateEdit->setDate(QDate::fromString(QString::fromStdString(m_editTransaction->getDate()), "yyyy-MM-dd"));
        m_amountEdit->setText(QString::number(m_editTransaction->getAmount(), 'f', 2));
        m_descriptionEdit->setText(QString::fromStdString(m_editTransaction->getDescription()));
        m_tagsEdit->setText(QString::fromStdString(m_editTransaction->getTags()));
        
        if (m_editTransaction->getType() == Transaction::Type::Income) {
            m_incomeRadio->setChecked(true);
        } else {
            m_expenseRadio->setChecked(true);
        }
        
        for (int i = 0; i < m_categoryCombo->count(); ++i) {
            if (m_categoryCombo->itemData(i).toInt() == m_editTransaction->getCategoryId()) {
                m_categoryCombo->setCurrentIndex(i);
                break;
            }
        }
        
        for (int i = 0; i < m_accountCombo->count(); ++i) {
            if (m_accountCombo->itemData(i).toInt() == m_editTransaction->getAccountId()) {
                m_accountCombo->setCurrentIndex(i);
                break;
            }
        }
    }
}

void AddTransactionDialog::setupUi() {
    setWindowTitle(m_isEditing ? "Edit Transaction" : "Add Transaction");
    resize(450, 400);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QGroupBox* detailsGroup = new QGroupBox("Transaction Details", this);
    QVBoxLayout* detailsLayout = new QVBoxLayout();
    
    QHBoxLayout* dateLayout = new QHBoxLayout();
    dateLayout->addWidget(new QLabel("Date:"));
    m_dateEdit = new QDateEdit(this);
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDisplayFormat("dd.MM.yyyy");
    m_dateEdit->setDate(QDate::currentDate());
    dateLayout->addWidget(m_dateEdit);
    detailsLayout->addLayout(dateLayout);
    
    QHBoxLayout* amountLayout = new QHBoxLayout();
    amountLayout->addWidget(new QLabel("Amount:"));
    m_amountEdit = new QLineEdit(this);
    m_amountEdit->setPlaceholderText("0.00");
    amountLayout->addWidget(m_amountEdit);
    detailsLayout->addLayout(amountLayout);
    
    QHBoxLayout* descLayout = new QHBoxLayout();
    descLayout->addWidget(new QLabel("Description:"));
    m_descriptionEdit = new QLineEdit(this);
    m_descriptionEdit->setPlaceholderText("Enter description...");
    descLayout->addWidget(m_descriptionEdit);
    detailsLayout->addLayout(descLayout);
    
    QHBoxLayout* typeLayout = new QHBoxLayout();
    typeLayout->addWidget(new QLabel("Type:"));
    m_incomeRadio = new QRadioButton("Income", this);
    m_expenseRadio = new QRadioButton("Expense", this);
    m_expenseRadio->setChecked(true);
    typeLayout->addWidget(m_incomeRadio);
    typeLayout->addWidget(m_expenseRadio);
    connect(m_incomeRadio, &QRadioButton::toggled, this, &AddTransactionDialog::onTypeChanged);
    detailsLayout->addLayout(typeLayout);
    
    QHBoxLayout* catLayout = new QHBoxLayout();
    catLayout->addWidget(new QLabel("Category:"));
    m_categoryCombo = new QComboBox(this);
    catLayout->addWidget(m_categoryCombo);
    detailsLayout->addLayout(catLayout);
    
    QHBoxLayout* accLayout = new QHBoxLayout();
    accLayout->addWidget(new QLabel("Account:"));
    m_accountCombo = new QComboBox(this);
    accLayout->addWidget(m_accountCombo);
    detailsLayout->addLayout(accLayout);
    
    QHBoxLayout* tagsLayout = new QHBoxLayout();
    tagsLayout->addWidget(new QLabel("Tags:"));
    m_tagsEdit = new QLineEdit(this);
    m_tagsEdit->setPlaceholderText("Comma separated tags...");
    tagsLayout->addWidget(m_tagsEdit);
    detailsLayout->addLayout(tagsLayout);
    
    detailsGroup->setLayout(detailsLayout);
    mainLayout->addWidget(detailsGroup);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_saveButton = new QPushButton("Save");
    m_cancelButton = new QPushButton("Cancel");
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_cancelButton);
    mainLayout->addLayout(buttonLayout);
    
    connect(m_saveButton, &QPushButton::clicked, this, &AddTransactionDialog::onSave);
    connect(m_cancelButton, &QPushButton::clicked, this, &AddTransactionDialog::onCancel);
}

void AddTransactionDialog::loadCategories() {
    auto categories = m_db.getAllCategories();
    m_categoryCombo->clear();
    
    bool isIncome = m_incomeRadio->isChecked();
    for (const auto& cat : categories) {
        if ((isIncome && cat.getType() == Category::Type::Income) ||
            (!isIncome && cat.getType() == Category::Type::Expense)) {
            m_categoryCombo->addItem(QString::fromStdString(cat.getName()), cat.getId());
        }
    }
    
    auto accounts = m_db.getAllAccounts();
    m_accountCombo->clear();
    for (const auto& acc : accounts) {
        m_accountCombo->addItem(QString::fromStdString(acc.getName()), acc.getId());
    }
    if (!accounts.empty()) {
        m_accountCombo->setCurrentIndex(0);
    }
}

void AddTransactionDialog::onSave() {
    QString dateStr = m_dateEdit->date().toString("yyyy-MM-dd");
    double amount = m_amountEdit->text().toDouble();
    QString desc = m_descriptionEdit->text();
    
    std::string error = Validator::validateTransaction(amount, desc.toStdString(), dateStr.toStdString());
    if (!error.empty()) {
        QMessageBox::warning(this, "Validation Error", QString::fromStdString(error));
        return;
    }
    
    Transaction t;
    if (m_isEditing && m_editTransaction) {
        t = *m_editTransaction;
    }
    
    t.setDate(dateStr.toStdString());
    t.setAmount(amount);
    t.setDescription(desc.toStdString());
    t.setType(m_incomeRadio->isChecked() ? Transaction::Type::Income : Transaction::Type::Expense);
    t.setCategoryId(m_categoryCombo->currentData().toInt());
    t.setAccountId(m_accountCombo->currentData().toInt());
    t.setTags(m_tagsEdit->text().toStdString());
    
    if (m_isEditing) {
        m_db.updateTransaction(t);
    } else {
        m_db.createTransaction(t);
    }
    
    accept();
}

void AddTransactionDialog::onCancel() {
    reject();
}

void AddTransactionDialog::onTypeChanged() {
    loadCategories();
}
