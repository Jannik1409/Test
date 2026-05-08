#ifndef CHARTTRACKERDIALOG_H
#define CHARTTRACKERDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDate>
#include <QPainter>

#include "../../database/database.h"
#include "../../models/transaction.h"

class ChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChartWidget(QWidget* parent = nullptr);
    void setData(const std::map<int, double>& dailyTotals, int daysInMonth);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    
private:
    std::map<int, double> m_dailyTotals;
    int m_daysInMonth;
};

class ChartTrackerDialog : public QDialog {
    Q_OBJECT
public:
    explicit ChartTrackerDialog(Database& db, QWidget* parent = nullptr);
    
private:
    QLabel* m_monthLabel;
    void updateMonthLabel();
    void loadChartData();
    void calculateDailyTotals();
    
    Database& m_db;
    ChartWidget* m_chart;
    QLabel* m_totalLabel;
    QLabel* m_changeLabel;
    QDate m_currentDate;
};

#endif // CHARTTRACKERDIALOG_H
