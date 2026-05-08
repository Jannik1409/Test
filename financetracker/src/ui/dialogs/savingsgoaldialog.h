#ifndef SAVINGSGOALDIALOG_H
#define SAVINGSGOALDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QDateEdit>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>

#include "../../database/database.h"
#include "../../models/savings_goal.h"

class SavingsGoalDialog : public QDialog {
    Q_OBJECT
public:
    explicit SavingsGoalDialog(Database& db, QWidget* parent = nullptr);
    
private slots:
    void onAdd();
    void onEdit();
    void onDelete();
    void onSelectionChanged();
    
private:
    void loadGoals();
    void updateButtons();
    
    Database& m_db;
    std::vector<SavingsGoal> m_goals;
    
    QListWidget* m_list;
    QLineEdit* m_nameEdit;
    QLineEdit* m_targetAmountEdit;
    QLineEdit* m_currentAmountEdit;
    QDateEdit* m_targetDateEdit;
    QProgressBar* m_progressBar;
    QLabel* m_progressLabel;
    QPushButton* m_addButton;
    QPushButton* m_editButton;
    QPushButton* m_deleteButton;
    QPushButton* m_closeButton;
};

#endif // SAVINGSGOALDIALOG_H