#include "savingsgoaldialog.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QDate>

SavingsGoalDialog::SavingsGoalDialog(Database& db, QWidget* parent)
    : QDialog(parent), m_db(db) {
    setWindowTitle("Savings Goals");
    resize(600, 450);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    m_list = new QListWidget(this);
    mainLayout->addWidget(m_list);
    
    QGridLayout* formLayout = new QGridLayout();
    
    formLayout->addWidget(new QLabel("Name:"), 0, 0);
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("e.g., Vacation Fund");
    formLayout->addWidget(m_nameEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Target Amount:"), 1, 0);
    m_targetAmountEdit = new QLineEdit(this);
    m_targetAmountEdit->setPlaceholderText("1000.00");
    formLayout->addWidget(m_targetAmountEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Current Amount:"), 2, 0);
    m_currentAmountEdit = new QLineEdit(this);
    m_currentAmountEdit->setPlaceholderText("0.00");
    formLayout->addWidget(m_currentAmountEdit, 2, 1);
    
    formLayout->addWidget(new QLabel("Target Date:"), 3, 0);
    m_targetDateEdit = new QDateEdit(this);
    m_targetDateEdit->setCalendarPopup(true);
    m_targetDateEdit->setDate(QDate::currentDate().addMonths(6));
    formLayout->addWidget(m_targetDateEdit, 3, 1);
    
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    mainLayout->addWidget(m_progressBar);
    
    m_progressLabel = new QLabel("Progress: $0 / $0", this);
    mainLayout->addWidget(m_progressLabel);
    
    mainLayout->addLayout(formLayout);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_addButton = new QPushButton("Add", this);
    m_editButton = new QPushButton("Edit", this);
    m_deleteButton = new QPushButton("Delete", this);
    m_closeButton = new QPushButton("Close", this);
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_editButton);
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_closeButton);
    mainLayout->addLayout(buttonLayout);
    
    loadGoals();
    
    connect(m_addButton, &QPushButton::clicked, this, &SavingsGoalDialog::onAdd);
    connect(m_editButton, &QPushButton::clicked, this, &SavingsGoalDialog::onEdit);
    connect(m_deleteButton, &QPushButton::clicked, this, &SavingsGoalDialog::onDelete);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_list, &QListWidget::itemSelectionChanged, this, &SavingsGoalDialog::onSelectionChanged);
}

void SavingsGoalDialog::loadGoals() {
    m_goals = m_db.getAllSavingsGoals();
    m_list->clear();
    for (const auto& goal : m_goals) {
        double progress = (goal.getTargetAmount() > 0) ? 
            (goal.getCurrentAmount() / goal.getTargetAmount() * 100) : 0;
        QString text = QString::fromStdString(goal.getName()) + " - " +
                      QString::number(progress, 'f', 0) + "%";
        QListWidgetItem* item = new QListWidgetItem(text, m_list);
        item->setData(Qt::UserRole, goal.getId());
    }
    updateButtons();
}

void SavingsGoalDialog::updateButtons() {
    bool hasSelection = m_list->currentRow() >= 0;
    m_editButton->setEnabled(hasSelection);
    m_deleteButton->setEnabled(hasSelection);
}

void SavingsGoalDialog::onAdd() {
    QString name = m_nameEdit->text();
    QString targetStr = m_targetAmountEdit->text();
    QString currentStr = m_currentAmountEdit->text();
    
    if (name.isEmpty() || targetStr.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter name and target amount.");
        return;
    }
    
    double targetAmount = targetStr.toDouble();
    double currentAmount = currentStr.isEmpty() ? 0.0 : currentStr.toDouble();
    
    if (targetAmount <= 0) {
        QMessageBox::warning(this, "Error", "Target amount must be greater than 0.");
        return;
    }
    
    SavingsGoal goal;
    goal.setName(name.toStdString());
    goal.setTargetAmount(targetAmount);
    goal.setCurrentAmount(currentAmount);
    goal.setTargetDate(m_targetDateEdit->date().toString("yyyy-MM-dd").toStdString());
    
    int id = m_db.createSavingsGoal(goal);
    if (id > 0) {
        loadGoals();
        m_nameEdit->clear();
        m_targetAmountEdit->clear();
        m_currentAmountEdit->clear();
    } else {
        QMessageBox::critical(this, "Error", "Failed to create savings goal.");
    }
}

void SavingsGoalDialog::onEdit() {
    int row = m_list->currentRow();
    if (row < 0) return;
    
    int id = m_list->currentItem()->data(Qt::UserRole).toInt();
    
    for (auto& goal : m_goals) {
        if (goal.getId() == id) {
            goal.setName(m_nameEdit->text().toStdString());
            goal.setTargetAmount(m_targetAmountEdit->text().toDouble());
            goal.setCurrentAmount(m_currentAmountEdit->text().toDouble());
            goal.setTargetDate(m_targetDateEdit->date().toString("yyyy-MM-dd").toStdString());
            m_db.updateSavingsGoal(goal);
            loadGoals();
            break;
        }
    }
}

void SavingsGoalDialog::onDelete() {
    int row = m_list->currentRow();
    if (row < 0) return;
    
    if (QMessageBox::question(this, "Confirm", "Delete this savings goal?") == QMessageBox::Yes) {
        int id = m_list->currentItem()->data(Qt::UserRole).toInt();
        m_db.deleteSavingsGoal(id);
        loadGoals();
    }
}

void SavingsGoalDialog::onSelectionChanged() {
    int row = m_list->currentRow();
    if (row < 0) {
        updateButtons();
        return;
    }
    
    int id = m_list->currentItem()->data(Qt::UserRole).toInt();
    
    for (const auto& goal : m_goals) {
        if (goal.getId() == id) {
            m_nameEdit->setText(QString::fromStdString(goal.getName()));
            m_targetAmountEdit->setText(QString::number(goal.getTargetAmount(), 'f', 2));
            m_currentAmountEdit->setText(QString::number(goal.getCurrentAmount(), 'f', 2));
            m_targetDateEdit->setDate(QDate::fromString(QString::fromStdString(goal.getTargetDate()), "yyyy-MM-dd"));
            
            double progress = (goal.getTargetAmount() > 0) ? 
                (goal.getCurrentAmount() / goal.getTargetAmount() * 100) : 0;
            m_progressBar->setValue((int)progress);
            m_progressLabel->setText(QString("Progress: $%1 / $%2 (%3%)")
                .arg(goal.getCurrentAmount(), 0, 'f', 2)
                .arg(goal.getTargetAmount(), 0, 'f', 2)
                .arg(progress, 0, 'f', 0));
            break;
        }
    }
    
    updateButtons();
}