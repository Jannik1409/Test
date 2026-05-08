#include "charttrackerdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDate>
#include <QFont>
#include <QPainter>
#include <QColor>
#include <QPainterPath>

int daysInCurrentMonth() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    int month = now->tm_mon + 1; // tm_mon is 0-11
    int year = now->tm_year + 1900; // tm_year is years since 1900

    switch(month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            // Leap year check
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
                return 29;
            else
                return 28;
        default:
            return 0; // should never happen
    }
}


ChartWidget::ChartWidget(QWidget* parent)
    : QWidget(parent), m_daysInMonth(daysInCurrentMonth()) {
    setMinimumHeight(200);
}

void ChartWidget::setData(const std::map<int, double>& dailyTotals, int daysInMonth) {
    m_dailyTotals = dailyTotals;
    m_daysInMonth = daysInMonth;
    update();
}

void ChartWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int width = this->width();
    int height = this->height();
    int padding = 20;
    
    double minVal = 0;
    double maxVal = 0;
    for (const auto& pair : m_dailyTotals) {
        if (pair.second < minVal) minVal = pair.second;
        if (pair.second > maxVal) maxVal = pair.second;
    }
    if (maxVal == minVal) maxVal = minVal + 1;
    if (maxVal <= 0) maxVal = 1;
    if (minVal > 0) minVal = 0;
    
    double range = maxVal - minVal;
    if (range == 0) range = 1;
    
    QDate today = QDate::currentDate();
    int todayDay = today.day();
    int currentMonth = today.month();
    int currentYear = today.year();
    
    QDate firstDayOfMonth(QDate::currentDate().year(), QDate::currentDate().month(), 1);
    int daysInCurrentMonth = firstDayOfMonth.daysInMonth();
    
    QPen gridPen(QColor(80, 80, 80), 1);
    gridPen.setStyle(Qt::DotLine);
    painter.setPen(gridPen);
    
    for (int i = 0; i <= 4; ++i) {
        int yPos = padding + (height - 2 * padding) * i / 4;
        painter.drawLine(padding, yPos, width - padding, yPos);
    }
    
    painter.setPen(QColor(100, 100, 100));
    painter.drawLine(width / 2, padding, width / 2, height - padding);
    
    QPen zeroLinePen(QColor(255, 255, 255, 100), 1);
    zeroLinePen.setStyle(Qt::SolidLine);
    int zeroY = padding + (height - 2 * padding) * (maxVal / range);
    if (zeroY >= padding && zeroY <= height - padding) {
        painter.setPen(zeroLinePen);
        painter.drawLine(padding, zeroY, width - padding, zeroY);
    }
    
    double prevX = -1, prevY = -1;
    
    for (int day = 1; day <= m_daysInMonth; ++day) {
        double xPos = padding + (width - 2 * padding) * (day - 1) / (m_daysInMonth - 1);
        
        double yPos;
        if (m_dailyTotals.find(day) != m_dailyTotals.end()) {
            yPos = padding + (height - 2 * padding) * (1 - (m_dailyTotals.at(day) - minVal) / range);
        } else {
            if (day <= todayDay && day <= daysInCurrentMonth && 
                QDate::currentDate().year() == currentYear && 
                QDate::currentDate().month() == currentMonth) {
                double prevDayTotal = 0;
                if (day > 1 && m_dailyTotals.find(day - 1) != m_dailyTotals.end()) {
                    prevDayTotal = m_dailyTotals.at(day - 1);
                }
                yPos = padding + (height - 2 * padding) * (1 - (prevDayTotal - minVal) / range);
            } else {
                continue;
            }
        }
        
        yPos = qMax((double)padding, qMin((double)(height - padding), yPos));
        
        if (prevX >= 0) {
            double prevValue = 0;
            double currentValue = 0;
            
            if (m_dailyTotals.find(day - 1) != m_dailyTotals.end()) {
                prevValue = m_dailyTotals.at(day - 1);
            }
            if (m_dailyTotals.find(day) != m_dailyTotals.end()) {
                currentValue = m_dailyTotals.at(day);
            }
            
            QColor lineColor = (currentValue >= prevValue) ? QColor(46, 204, 113) : QColor(231, 76, 60);
            painter.setPen(QPen(lineColor, 2));
            painter.drawLine(QPointF(prevX, prevY), QPointF(xPos, yPos));
        }
        
        prevX = xPos;
        prevY = yPos;
    }
    
    prevX = -1;
    prevY = -1;
    
    for (int day = 1; day <= m_daysInMonth; ++day) {
        double xPos = padding + (width - 2 * padding) * (day - 1) / (m_daysInMonth - 1);
        
        double yPos;
        if (m_dailyTotals.find(day) != m_dailyTotals.end()) {
            yPos = padding + (height - 2 * padding) * (1 - (m_dailyTotals.at(day) - minVal) / range);
        } else {
            if (day <= todayDay && day <= daysInCurrentMonth &&
                QDate::currentDate().year() == currentYear && 
                QDate::currentDate().month() == currentMonth) {
                double prevDayTotal = 0;
                if (day > 1 && m_dailyTotals.find(day - 1) != m_dailyTotals.end()) {
                    prevDayTotal = m_dailyTotals.at(day - 1);
                }
                yPos = padding + (height - 2 * padding) * (1 - (prevDayTotal - minVal) / range);
            } else {
                continue;
            }
        }
        
        yPos = qMax((double)padding, qMin((double)(height - padding), yPos));
        
        double currentValue = 0;
        if (m_dailyTotals.find(day) != m_dailyTotals.end()) {
            currentValue = m_dailyTotals.at(day);
        }
        
        QColor pointColor = (currentValue >= 0) ? QColor(46, 204, 113) : QColor(231, 76, 60);
        painter.setPen(pointColor);
        painter.setBrush(pointColor);
        painter.drawEllipse(QPointF(xPos, yPos), 4, 4);
        
        if (day == todayDay) {
            painter.setPen(QColor(255, 255, 0));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(QPointF(xPos, yPos), 6, 6);
        }
        
        prevX = xPos;
        prevY = yPos;
    }
    
    painter.setPen(QColor(150, 150, 150));
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);
    
    for (int day = 5; day <= m_daysInMonth; day += 5) {
        double xPos = padding + (width - 2 * padding) * (day - 1) / (m_daysInMonth - 1);
        painter.drawText(QPointF(xPos - 5, height - 5), QString::number(day));
    }
    
    double step = range / 4;
    for (int i = 0; i <= 4; ++i) {
        double val = maxVal - step * i;
        int yPos = padding + (height - 2 * padding) * i / 4;
        QString label = QString::number(val, 'f', 0);
        painter.drawText(QPointF(2, yPos + 4), label);
    }
}

ChartTrackerDialog::ChartTrackerDialog(Database& db, QWidget* parent)
    : QDialog(parent), m_db(db), m_currentDate(QDate::currentDate()) {
    setWindowTitle("Daily Spending Chart");
    resize(700, 350);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QHBoxLayout* headerLayout = new QHBoxLayout();
    m_totalLabel = new QLabel("Total: $0.00", this);
    m_totalLabel->setFont(QFont("Arial", 12, QFont::Bold));
    m_changeLabel = new QLabel("Today: $0.00 (No change)", this);
    m_changeLabel->setFont(QFont("Arial", 10));
    headerLayout->addWidget(m_totalLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(m_changeLabel);
    mainLayout->addLayout(headerLayout);
    
    m_chart = new ChartWidget(this);
    mainLayout->addWidget(m_chart);
    
    QPushButton* closeBtn = new QPushButton("Close", this);
    mainLayout->addWidget(closeBtn);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    
    loadChartData();
}

void ChartTrackerDialog::loadChartData() {
    QDate firstDay(m_currentDate.year(), m_currentDate.month(), 1);
    QDate lastDay = firstDay.addMonths(1).addDays(-1);
    int daysInMonth = lastDay.day();
    
    auto allTrans = m_db.getAllTransactions();
    
    std::map<int, double> dailyTotals;
    double runningTotal = 0;
    
    for (int day = 1; day <= daysInMonth; ++day) {
        QDate d(m_currentDate.year(), m_currentDate.month(), day);
        QString dateStr = d.toString("yyyy-MM-dd");
        
        for (const auto& t : allTrans) {
            if (t.getDate() == dateStr.toStdString()) {
                double amount = (t.getType() == Transaction::Type::Income) ? t.getAmount() : -t.getAmount();
                runningTotal += amount;
            }
        }
        
        if (runningTotal != 0 || d == QDate::currentDate()) {
            dailyTotals[day] = runningTotal;
        }
    }
    
    m_chart->setData(dailyTotals, daysInMonth);
    
    m_totalLabel->setText(QString("Month Total: $%1").arg(runningTotal, 0, 'f', 2));
    
    QDate yesterday = QDate::currentDate().addDays(-1);
    double yesterdayTotal = 0;
    if (yesterday.month() == m_currentDate.month() && yesterday.year() == m_currentDate.year()) {
        if (dailyTotals.find(yesterday.day()) != dailyTotals.end()) {
            yesterdayTotal = dailyTotals[yesterday.day()];
        }
    }
    
    double todayTotal = 0;
    int todayDay = QDate::currentDate().day();
    if (QDate::currentDate().month() == m_currentDate.month() && 
        QDate::currentDate().year() == m_currentDate.year()) {
        if (dailyTotals.find(todayDay) != dailyTotals.end()) {
            todayTotal = dailyTotals[todayDay];
        }
    }
    
    double todayChange = todayTotal - yesterdayTotal;
    QString changeText = QString("Today: $%1").arg(todayTotal, 0, 'f', 2);
    if (todayChange > 0) {
        changeText += QString(" (+$%1)").arg(todayChange, 0, 'f', 2);
        m_changeLabel->setStyleSheet("color: #2ECC71;");
    } else if (todayChange < 0) {
        changeText += QString(" (-$%1)").arg(-todayChange, 0, 'f', 2);
        m_changeLabel->setStyleSheet("color: #E74C3C;");
    } else {
        changeText += " (No change)";
    }
    m_changeLabel->setText(changeText);
}
