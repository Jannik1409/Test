#ifndef SUMMARYSIDEBAR_H
#define SUMMARYSIDEBAR_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>

class SummarySidebar : public QWidget {
    Q_OBJECT
public:
    explicit SummarySidebar(QWidget* parent = nullptr);
    void updateSummary(double income, double expenses, double balance);

private:
    QLabel* m_incomeLabel;
    QLabel* m_expensesLabel;
    QLabel* m_balanceLabel;
};

#endif // SUMMARYSIDEBAR_H