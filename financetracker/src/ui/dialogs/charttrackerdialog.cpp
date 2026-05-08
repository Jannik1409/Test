#include "charttrackerdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QDate>
#include <QPainter>
#include <QString>
#include <map>
#include <algorithm>
#include <cmath>

int daysInMonth(int year, int month) {
    switch(month) {
       case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
        case 4: case 6: case 9: case 11: return 30;
        case 2:
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
                return 29;
            else
                return 28;
        default: return 0;
    }
}

ChartWidget::ChartWidget(QWidget* parent)
    : QWidget(parent), m_daysInMonth(31) {
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

    double minVal = 0, maxVal = 0;
    for (const auto& p : m_dailyTotals) {
        minVal = std::min(minVal, p.second);
        maxVal = std::max(maxVal, p.second);
    }
    if (maxVal == minVal) maxVal = minVal + 1;
    if (maxVal <= 0) maxVal = 1;
    if (minVal > 0) minVal = 0;
    double range = maxVal - minVal;
    if (range == 0) range = 1;

    QPen gridPen(QColor(80,80,80),1,Qt::DotLine);
    painter.setPen(gridPen);
    for (int i=0;i<=4;++i){
        int y = padding + (height-2*padding)*i/4;
        painter.drawLine(padding,y,width-padding,y);
    }

    int zeroY = padding + (height-2*padding)*(maxVal/range);
    if(zeroY>=padding && zeroY<=height-padding){
        QPen zeroPen(QColor(255,255,255,100));
        painter.setPen(zeroPen);
        painter.drawLine(padding,zeroY,width-padding,zeroY);
    }

    double prevX=-1, prevY=-1;
    for(int day=1; day<=m_daysInMonth; ++day){
        if(m_dailyTotals.find(day)==m_dailyTotals.end()) continue;
        double val = m_dailyTotals[day];
        double x = padding + (width-2*padding)*(day-1)/(m_daysInMonth-1);
        double y = padding + (height-2*padding)*(1-(val-minVal)/range);
        if(prevX>=0){
            QColor lineColor = (val>=m_dailyTotals[day-1])?QColor(46,204,113):QColor(231,76,60);
            painter.setPen(QPen(lineColor,2));
            painter.drawLine(QPointF(prevX,prevY),QPointF(x,y));
        }
        prevX=x; prevY=y;
    }

    for(int day=1; day<=m_daysInMonth; ++day){
        if(m_dailyTotals.find(day)==m_dailyTotals.end()) continue;
        double val = m_dailyTotals[day];
        double x = padding + (width-2*padding)*(day-1)/(m_daysInMonth-1);
        double y = padding + (height-2*padding)*(1-(val-minVal)/range);

        QColor color = (val>=0)?QColor(46,204,113):QColor(231,76,60);
        painter.setPen(color);
        painter.setBrush(color);
        painter.drawEllipse(QPointF(x,y),4,4);

        QDate today = QDate::currentDate();
        if(today.day()==day && today.month()==today.month() && today.year()==today.year()){
            painter.setPen(QColor(255,255,0));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(QPointF(x,y),6,6);
        }
    }

    painter.setPen(QColor(150,150,150));
    QFont font = painter.font(); font.setPointSize(8); painter.setFont(font);
    for(int day=5; day<=m_daysInMonth; day+=5){
        double x = padding + (width-2*padding)*(day-1)/(m_daysInMonth-1);
        painter.drawText(QPointF(x-5,height-5),QString::number(day));
    }

    double step = range/4;
    for(int i=0;i<=4;++i){
        double val = maxVal - step*i;
        int y = padding + (height-2*padding)*i/4;
        painter.drawText(QPointF(2,y+4),QString::number(val,'f',0));
    }
}

ChartTrackerDialog::ChartTrackerDialog(Database& db, QWidget* parent)
    : QDialog(parent), m_db(db), m_currentDate(QDate::currentDate()) {

    setWindowTitle("Daily Spending Chart");
    resize(700,350);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* headerLayout = new QHBoxLayout();

    QPushButton* prevBtn = new QPushButton("<",this);
    QPushButton* nextBtn = new QPushButton(">",this);
    m_monthLabel = new QLabel(this);
    m_monthLabel->setAlignment(Qt::AlignCenter);
    m_monthLabel->setFont(QFont("Arial",12,QFont::Bold));
    updateMonthLabel();

    connect(prevBtn,&QPushButton::clicked,this,[this](){
        m_currentDate = m_currentDate.addMonths(-1);
        updateMonthLabel();
        loadChartData();
    });
    connect(nextBtn,&QPushButton::clicked,this,[this](){
        m_currentDate = m_currentDate.addMonths(1);
        updateMonthLabel();
        loadChartData();
    });

    m_totalLabel = new QLabel("Total: $0.00",this);
    m_totalLabel->setFont(QFont("Arial",12,QFont::Bold));
    m_changeLabel = new QLabel("Today: $0.00 (No change)",this);
    m_changeLabel->setFont(QFont("Arial",10));

    headerLayout->addWidget(prevBtn);
    headerLayout->addWidget(m_monthLabel);
    headerLayout->addWidget(nextBtn);
    headerLayout->addStretch();
    headerLayout->addWidget(m_totalLabel);
    headerLayout->addWidget(m_changeLabel);
    mainLayout->addLayout(headerLayout);

    m_chart = new ChartWidget(this);
    mainLayout->addWidget(m_chart);

    QPushButton* closeBtn = new QPushButton("Close",this);
    mainLayout->addWidget(closeBtn);
    connect(closeBtn,&QPushButton::clicked,this,&QDialog::accept);

    loadChartData();
}

void ChartTrackerDialog::updateMonthLabel(){
    m_monthLabel->setText(m_currentDate.toString("MMMM yyyy"));
}

void ChartTrackerDialog::loadChartData(){
    QDate firstDay(m_currentDate.year(),m_currentDate.month(),1);
    int days = firstDay.daysInMonth();

    auto allTrans = m_db.getAllTransactions();
    std::map<int,double> dailyTotals;
    double runningTotal=0;

    for(int day=1; day<=days; ++day){
        QDate d(m_currentDate.year(),m_currentDate.month(),day);
        QString dateStr = d.toString("yyyy-MM-dd");
        for(const auto& t: allTrans){
            if(t.getDate()==dateStr.toStdString()){
                double amt = (t.getType()==Transaction::Type::Income)? t.getAmount() : -t.getAmount();
                runningTotal+=amt;
            }
        }
        if(runningTotal!=0 || d==QDate::currentDate()) dailyTotals[day]=runningTotal;
    }

    m_chart->setData(dailyTotals,days);
    m_totalLabel->setText(QString("Month Total: $%1").arg(runningTotal,0,'f',2));

    QDate yesterday = QDate::currentDate().addDays(-1);
    double yesterdayTotal=0;
    if(yesterday.month()==m_currentDate.month() && yesterday.year()==m_currentDate.year()){
        if(dailyTotals.find(yesterday.day())!=dailyTotals.end()) yesterdayTotal=dailyTotals[yesterday.day()];
    }

    double todayTotal=0;
    int todayDay=QDate::currentDate().day();
    if(QDate::currentDate().month()==m_currentDate.month() &&
       QDate::currentDate().year()==m_currentDate.year()){
        if(dailyTotals.find(todayDay)!=dailyTotals.end()) todayTotal=dailyTotals[todayDay];
    }

    double change = todayTotal - yesterdayTotal;
    QString text = QString("Today: $%1").arg(todayTotal,0,'f',2);
    if(change>0){ text+=QString(" (+$%1)").arg(change,0,'f',2); m_changeLabel->setStyleSheet("color:#2ECC71;");}
    else if(change<0){ text+=QString(" (-$%1)").arg(-change,0,'f',2); m_changeLabel->setStyleSheet("color:#E74C3C;");}
    else{text+=" (No change)"; m_changeLabel->setStyleSheet("");}
    m_changeLabel->setText(text);
}
