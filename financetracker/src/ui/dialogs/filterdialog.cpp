#include "filterdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

FilterDialog::FilterDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Filter Transactions");
    resize(400, 300);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QHBoxLayout* dateRangeLayout = new QHBoxLayout();
    dateRangeLayout->addWidget(new QLabel("From:"));
    m_startDateEdit = new QDateEdit(this);
    m_startDateEdit->setCalendarPopup(true);
    m_startDateEdit->setDate(QDate::currentDate().addMonths(-1));
    dateRangeLayout->addWidget(m_startDateEdit);
    dateRangeLayout->addWidget(new QLabel("To:"));
    m_endDateEdit = new QDateEdit(this);
    m_endDateEdit->setCalendarPopup(true);
    m_endDateEdit->setDate(QDate::currentDate());
    dateRangeLayout->addWidget(m_endDateEdit);
    mainLayout->addLayout(dateRangeLayout);
    
    QHBoxLayout* categoryLayout = new QHBoxLayout();
    categoryLayout->addWidget(new QLabel("Category:"));
    m_categoryCombo = new QComboBox(this);
    m_categoryCombo->addItem("All", 0);
    categoryLayout->addWidget(m_categoryCombo);
    mainLayout->addLayout(categoryLayout);
    
    QHBoxLayout* typeLayout = new QHBoxLayout();
    typeLayout->addWidget(new QLabel("Type:"));
    m_typeCombo = new QComboBox(this);
    m_typeCombo->addItem("All", "All");
    m_typeCombo->addItem("Income", "Income");
    m_typeCombo->addItem("Expense", "Expense");
    typeLayout->addWidget(m_typeCombo);
    mainLayout->addLayout(typeLayout);
    
    QHBoxLayout* amountLayout = new QHBoxLayout();
    amountLayout->addWidget(new QLabel("Min Amount:"));
    m_minAmountEdit = new QLineEdit(this);
    m_minAmountEdit->setPlaceholderText("0.00");
    amountLayout->addWidget(m_minAmountEdit);
    amountLayout->addWidget(new QLabel("Max Amount:"));
    m_maxAmountEdit = new QLineEdit(this);
    m_maxAmountEdit->setPlaceholderText("999999.99");
    amountLayout->addWidget(m_maxAmountEdit);
    mainLayout->addLayout(amountLayout);
    
    mainLayout->addStretch();
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* applyBtn = new QPushButton("Apply", this);
    QPushButton* resetBtn = new QPushButton("Reset", this);
    QPushButton* cancelBtn = new QPushButton("Cancel", this);
    buttonLayout->addWidget(applyBtn);
    buttonLayout->addWidget(resetBtn);
    buttonLayout->addWidget(cancelBtn);
    mainLayout->addLayout(buttonLayout);
    
    connect(applyBtn, &QPushButton::clicked, this, &FilterDialog::onApply);
    connect(resetBtn, &QPushButton::clicked, this, &FilterDialog::onReset);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

QString FilterDialog::getStartDate() const {
    return m_startDateEdit->date().toString("yyyy-MM-dd");
}

QString FilterDialog::getEndDate() const {
    return m_endDateEdit->date().toString("yyyy-MM-dd");
}

QString FilterDialog::getCategory() const {
    return m_categoryCombo->currentText();
}

QString FilterDialog::getType() const {
    return m_typeCombo->currentData().toString();
}

QString FilterDialog::getMinAmount() const {
    return m_minAmountEdit->text();
}

QString FilterDialog::getMaxAmount() const {
    return m_maxAmountEdit->text();
}

void FilterDialog::onApply() {
    accept();
}

void FilterDialog::onReset() {
    m_startDateEdit->setDate(QDate::currentDate().addMonths(-1));
    m_endDateEdit->setDate(QDate::currentDate());
    m_categoryCombo->setCurrentIndex(0);
    m_typeCombo->setCurrentIndex(0);
    m_minAmountEdit->clear();
    m_maxAmountEdit->clear();
}