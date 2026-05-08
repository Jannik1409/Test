#include "summarysidebar.h"
#include <QVBoxLayout>

SummarySidebar::SummarySidebar(QWidget* parent)
    : QWidget(parent) {
    setFixedWidth(200);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    QLabel* title = new QLabel("Monthly Summary", this);
    title->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(title);
    
    layout->addSpacing(10);
    
    m_incomeLabel = new QLabel("Income: $0.00", this);
    m_incomeLabel->setStyleSheet("color: #27AE60;");
    layout->addWidget(m_incomeLabel);
    
    m_expensesLabel = new QLabel("Expenses: $0.00", this);
    m_expensesLabel->setStyleSheet("color: #E74C3C;");
    layout->addWidget(m_expensesLabel);
    
    layout->addSpacing(10);
    
    m_balanceLabel = new QLabel("Balance: $0.00", this);
    m_balanceLabel->setStyleSheet("font-weight: bold;");
    layout->addWidget(m_balanceLabel);
    
    layout->addStretch();
}

void SummarySidebar::updateSummary(double income, double expenses, double balance) {
    m_incomeLabel->setText(QString("Income: $%1").arg(income, 0, 'f', 2));
    m_expensesLabel->setText(QString("Expenses: $%1").arg(expenses, 0, 'f', 2));
    m_balanceLabel->setText(QString("Balance: $%1").arg(balance, 0, 'f', 2));
    
    if (balance >= 0) {
        m_balanceLabel->setStyleSheet("font-weight: bold; color: #27AE60;");
    } else {
        m_balanceLabel->setStyleSheet("font-weight: bold; color: #E74C3C;");
    }
}