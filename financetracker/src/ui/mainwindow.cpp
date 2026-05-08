#include "mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QHeaderView>
#include <QDate>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QWidget>
#include <algorithm>

#include "dialogs/addtransactiondialog.h"
#include "dialogs/categorydialog.h"
#include "dialogs/accountdialog.h"
#include "dialogs/recurringdialog.h"
#include "dialogs/savingsgoaldialog.h"
#include "dialogs/settingsdialog.h"
#include "dialogs/filterdialog.h"
#include "dialogs/charttrackerdialog.h"
#include "dialogs/tagsdialog.h"

#include "../services/json_handler.h"
#include "../services/csv_importer.h"
#include "../services/pdf_generator.h"
#include "../services/theme_manager.h"
#include "../utils/validator.h"
#include "../utils/dateutils.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_currentAccountId(0), m_currentDate(QDate::currentDate()), m_db("data/finance.db") {
    setupUi();
    
    if (!m_db.open()) {
        QMessageBox::critical(this, "Error", "Failed to open database!");
        return;
    }
    
    refreshData();
    loadTransactionsForMonth();
}

MainWindow::~MainWindow() {
    m_db.close();
}

void MainWindow::setupUi() {
    setWindowTitle("Finance Tracker");
    resize(1200, 700);
    
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setStretchFactor(mainLayout, 1);
    
    QWidget* leftWidget = new QWidget(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    
    QHBoxLayout* monthLayout = new QHBoxLayout();
    QPushButton* prevMonthBtn = new QPushButton("<", this);
    prevMonthBtn->setMaximumWidth(30);
    m_monthLabel = new QLabel(this);
    QPushButton* nextMonthBtn = new QPushButton(">", this);
    nextMonthBtn->setMaximumWidth(30);
    monthLayout->addWidget(prevMonthBtn);
    monthLayout->addWidget(m_monthLabel);
    monthLayout->addWidget(nextMonthBtn);
    leftLayout->addLayout(monthLayout);
    
    m_summarySidebar = new SummarySidebar(this);
    leftLayout->addWidget(m_summarySidebar);
    
    leftWidget->setMinimumWidth(250);
    leftWidget->setMaximumWidth(350);
    mainLayout->addWidget(leftWidget, 1);
    
    QWidget* rightWidget = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    
    QHBoxLayout* toolbarLayout = new QHBoxLayout();
    m_accountCombo = new QComboBox(this);
    toolbarLayout->addWidget(new QLabel("Account:", this));
    toolbarLayout->addWidget(m_accountCombo);
    
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Search transactions...");
    toolbarLayout->addWidget(m_searchEdit);
    
    QPushButton* searchBtn = new QPushButton("Search", this);
    toolbarLayout->addWidget(searchBtn);
    
    QPushButton* filterBtn = new QPushButton("Filter", this);
    toolbarLayout->addWidget(filterBtn);
    
    toolbarLayout->addStretch();
    rightLayout->addLayout(toolbarLayout);
    
    m_transactionTable = new QTableWidget(this);
    m_transactionTable->setColumnCount(6);
    m_transactionTable->setHorizontalHeaderLabels({"Date", "Description", "Category", "Type", "Amount", "Tags"});
    m_transactionTable->horizontalHeader()->setStretchLastSection(true);
    m_transactionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_transactionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_transactionTable->setAlternatingRowColors(true);
    rightLayout->addWidget(m_transactionTable);
    
    mainLayout->addWidget(rightWidget, 3);
    
    setCentralWidget(centralWidget);
    
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    createConnections();
    
    updateMonthLabel();
    
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::onSearch);
    connect(filterBtn, &QPushButton::clicked, this, &MainWindow::onFilter);
    connect(m_accountCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onAccountChanged);
    connect(m_transactionTable, &QTableWidget::itemSelectionChanged, 
            this, &MainWindow::onTransactionSelectionChanged);
    connect(prevMonthBtn, &QPushButton::clicked, this, &MainWindow::onPrevMonth);
    connect(nextMonthBtn, &QPushButton::clicked, this, &MainWindow::onNextMonth);
}

void MainWindow::setupMenuBar() {
    QMenuBar* menuBar = this->menuBar();
    
    QMenu* fileMenu = menuBar->addMenu("&File");
    fileMenu->addAction("&Export to JSON", this, &MainWindow::onExportJson);
    fileMenu->addAction("&Import from JSON", this, &MainWindow::onImportJson);
    fileMenu->addSeparator();
    fileMenu->addAction("Export to &CSV", this, &MainWindow::onExportCsv);
    fileMenu->addAction("&Import from CSV", this, &MainWindow::onImportCsv);
    fileMenu->addSeparator();
    fileMenu->addAction("&Generate PDF Report", this, &MainWindow::onGeneratePdfReport);
    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", this, &QWidget::close);
    
    QMenu* transactionMenu = menuBar->addMenu("&Transaction");
    transactionMenu->addAction("&Add Transaction", this, &MainWindow::onAddTransaction);
    transactionMenu->addAction("&Edit Transaction", this, &MainWindow::onEditTransaction);
    transactionMenu->addAction("&Delete Transaction", this, &MainWindow::onDeleteTransaction);
    
    QMenu* manageMenu = menuBar->addMenu("&Manage");
    manageMenu->addAction("&Categories", this, &MainWindow::onManageCategories);
    manageMenu->addAction("&Accounts", this, &MainWindow::onManageAccounts);
    manageMenu->addAction("&Recurring", this, &MainWindow::onManageRecurring);
    manageMenu->addAction("&Savings Goals", this, &MainWindow::onManageSavingsGoals);
    manageMenu->addAction("&Tags", this, &MainWindow::onManageTags);
    
    QMenu* viewMenu = menuBar->addMenu("&View");
    viewMenu->addAction("&Toggle Theme", this, &MainWindow::onToggleTheme);
    viewMenu->addAction("&Settings", this, &MainWindow::onSettings);
}

void MainWindow::setupToolBar() {
    QToolBar* toolbar = addToolBar("Main Toolbar");
    toolbar->setMovable(false);
    
    QAction* addAction = toolbar->addAction("Add Transaction");
    connect(addAction, &QAction::triggered, this, &MainWindow::onAddTransaction);
    
    QAction* editAction = toolbar->addAction("Edit");
    connect(editAction, &QAction::triggered, this, &MainWindow::onEditTransaction);
    
    QAction* deleteAction = toolbar->addAction("Delete");
    connect(deleteAction, &QAction::triggered, this, &MainWindow::onDeleteTransaction);
    
    toolbar->addSeparator();
    
    QAction* categoryAction = toolbar->addAction("Categories");
    connect(categoryAction, &QAction::triggered, this, &MainWindow::onManageCategories);
    
    QAction* statsAction = toolbar->addAction("Reports");
    connect(statsAction, &QAction::triggered, this, &MainWindow::onGeneratePdfReport);
    
    QAction* chartAction = toolbar->addAction("Chart");
    connect(chartAction, &QAction::triggered, this, &MainWindow::onShowChart);
}

void MainWindow::setupStatusBar() {
    m_statusLabel = new QLabel("Ready", this);
    statusBar()->addWidget(m_statusLabel);
}

void MainWindow::createConnections() {
}

void MainWindow::refreshData() {
    m_accounts = m_db.getAllAccounts();
    m_categories = m_db.getAllCategories();
    
    m_accountCombo->clear();
    m_accountCombo->addItem("All Accounts", 0);
    for (const auto& acc : m_accounts) {
    m_accountCombo->addItem(QString::fromStdString(acc.getName()), acc.getId());
    }
    
    if (!m_accounts.empty()) {
        m_currentAccountId = m_accounts[0].getId();
    }
    
    refreshTransactions();
    refreshStatistics();
}

void MainWindow::refreshTransactions() {
    m_transactionTable->setRowCount(m_transactions.size());
    
    for (size_t i = 0; i < m_transactions.size(); ++i) {
        const auto& t = m_transactions[i];
        
        m_transactionTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(DateUtils::formatDate(t.getDate()))));
        m_transactionTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(t.getDescription())));
        
        QString categoryName = "-";
        for (const auto& c : m_categories) {
            if (c.getId() == t.getCategoryId()) {
                categoryName = QString::fromStdString(c.getName());
                break;
            }
        }
        m_transactionTable->setItem(i, 2, new QTableWidgetItem(categoryName));
        
        QString typeStr = t.getType() == Transaction::Type::Income ? "Income" : "Expense";
        m_transactionTable->setItem(i, 3, new QTableWidgetItem(typeStr));
        
        QString amountStr = QString::number(t.getAmount(), 'f', 2);
        if (t.getType() == Transaction::Type::Expense) {
            amountStr = "-" + amountStr;
        }
        QTableWidgetItem* amountItem = new QTableWidgetItem(amountStr);
        if (t.getType() == Transaction::Type::Income) {
            amountItem->setForeground(QColor(ThemeManager::incomeColor().c_str()));
        } else {
            amountItem->setForeground(QColor(ThemeManager::expenseColor().c_str()));
        }
        m_transactionTable->setItem(i, 4, amountItem);
        
        m_transactionTable->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(t.getTags())));
    }
    
    m_statusLabel->setText(QString("Ready | Records: %1").arg(m_transactions.size()));
}

void MainWindow::refreshStatistics() {
    double income = m_stats.calculateTotalIncome(m_transactions);
    double expenses = m_stats.calculateTotalExpenses(m_transactions);
    
    auto allTransactions = m_db.getAllTransactions();
    double totalBalance = m_stats.calculateBalance(allTransactions);
    
    m_summarySidebar->updateSummary(income, expenses, totalBalance);
}

void MainWindow::onAddTransaction() {
    AddTransactionDialog dialog(m_db, this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshData();
    }
}

void MainWindow::onEditTransaction() {
    int row = m_transactionTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a transaction to edit.");
        return;
    }
    
    const Transaction& t = m_transactions[row];
    AddTransactionDialog dialog(m_db, &t, this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshData();
    }
}

void MainWindow::onDeleteTransaction() {
    int row = m_transactionTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a transaction to delete.");
        return;
    }
    
    if (QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this transaction?") 
        == QMessageBox::Yes) {
        const Transaction& t = m_transactions[row];
        m_db.deleteTransaction(t.getId());
        refreshData();
    }
}

void MainWindow::onManageCategories() {
    CategoryDialog dialog(m_db, this);
    dialog.exec();
    refreshData();
}

void MainWindow::onManageAccounts() {
    AccountDialog dialog(m_db, this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshData();
    }
}

void MainWindow::onManageRecurring() {
    RecurringDialog dialog(m_db, this);
    dialog.exec();
}

void MainWindow::onManageSavingsGoals() {
    SavingsGoalDialog dialog(m_db, this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshData();
    }
}

void MainWindow::onManageTags() {
    TagsDialog dialog(m_db, this);
    dialog.exec();
}

void MainWindow::onExportJson() {
    QString fileName = QFileDialog::getSaveFileName(this, "Export to JSON", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;
    
    std::vector<SavingsGoal> goals = m_db.getAllSavingsGoals();
    std::vector<Tag> tags = m_db.getAllTags();
    
    if (JsonHandler::exportToFile(fileName.toStdString(), m_transactions, m_categories, 
                                   m_accounts, goals, tags)) {
        QMessageBox::information(this, "Success", "Data exported successfully!");
    } else {
        QMessageBox::critical(this, "Error", "Failed to export data!");
    }
}

void MainWindow::onImportJson() {
    QString fileName = QFileDialog::getOpenFileName(this, "Import from JSON", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;
    
    std::vector<Transaction> transactions;
    std::vector<Category> categories;
    std::vector<Account> accounts;
    std::vector<SavingsGoal> goals;
    std::vector<Tag> tags;
    
    if (JsonHandler::importFromFile(fileName.toStdString(), transactions, categories, accounts, goals, tags)) {
        for (const auto& t : transactions) {
            m_db.createTransaction(t);
        }
        QMessageBox::information(this, "Success", "Data imported successfully!");
        refreshData();
    } else {
        QMessageBox::critical(this, "Error", "Failed to import data!");
    }
}

void MainWindow::onExportCsv() {
    QMessageBox::information(this, "Export CSV", "CSV export coming soon!");
}

void MainWindow::onImportCsv() {
    QMessageBox::information(this, "Import CSV", "CSV import coming soon!");
}

void MainWindow::onGeneratePdfReport() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF Report", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;
    
    if (PdfGenerator::generateReport(fileName.toStdString(), m_transactions, m_categories,
                                      "Finance Report", "2024-01-01", "2024-12-31")) {
        QMessageBox::information(this, "Success", "PDF report generated!");
    } else {
        QMessageBox::critical(this, "Error", "Failed to generate PDF!");
    }
}

void MainWindow::onToggleTheme() {
    ThemeManager::Theme current = ThemeManager::getCurrentTheme();
    ThemeManager::setTheme(current == ThemeManager::Theme::Dark ? 
                          ThemeManager::Theme::Light : ThemeManager::Theme::Dark);
}

void MainWindow::onSettings() {
    SettingsDialog dialog(this);
    dialog.exec();
    refreshTransactions();
}

void MainWindow::onSearch() {
    QString query = m_searchEdit->text();
    if (query.isEmpty()) {
        loadTransactionsForMonth();
    } else {
        QDate firstDay(m_currentDate.year(), m_currentDate.month(), 1);
        QDate lastDay = firstDay.addMonths(1).addDays(-1);
        
        QString startDate = firstDay.toString("yyyy-MM-dd");
        QString endDate = lastDay.toString("yyyy-MM-dd");
        
        auto allTrans = m_db.getTransactionsByDateRange(startDate.toStdString(), endDate.toStdString());
        
        m_transactions.clear();
        std::string searchLower = query.toLower().toStdString();
        for (const auto& t : allTrans) {
            std::string desc = t.getDescription();
            std::transform(desc.begin(), desc.end(), desc.begin(), ::tolower);
            if (desc.find(searchLower) != std::string::npos) {
                m_transactions.push_back(t);
            }
        }
        refreshTransactions();
    }
}

void MainWindow::onFilter() {
    FilterDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString startDate = dialog.getStartDate();
        QString endDate = dialog.getEndDate();
        
        if (!startDate.isEmpty() && !endDate.isEmpty()) {
            auto allTrans = m_db.getTransactionsByDateRange(startDate.toStdString(), endDate.toStdString());
            
            QString typeFilter = dialog.getType();
            QString minAmountStr = dialog.getMinAmount();
            QString maxAmountStr = dialog.getMaxAmount();
            
            m_transactions.clear();
            for (const auto& t : allTrans) {
                bool typeMatch = (typeFilter == "All" || 
                    (typeFilter == "Income" && t.getType() == Transaction::Type::Income) ||
                    (typeFilter == "Expense" && t.getType() == Transaction::Type::Expense));
                
                bool amountMatch = true;
                if (!minAmountStr.isEmpty() && t.getAmount() < minAmountStr.toDouble()) {
                    amountMatch = false;
                }
                if (!maxAmountStr.isEmpty() && t.getAmount() > maxAmountStr.toDouble()) {
                    amountMatch = false;
                }
                
                if (typeMatch && amountMatch) {
                    m_transactions.push_back(t);
                }
            }
            refreshTransactions();
        }
    }
}

void MainWindow::onAccountChanged(int index) {
    m_currentAccountId = m_accountCombo->currentData().toInt();
    loadTransactionsForMonth();
}

void MainWindow::onTransactionSelectionChanged() {
}

void MainWindow::onPrevMonth() {
    m_currentDate = m_currentDate.addMonths(-1);
    updateMonthLabel();
    loadTransactionsForMonth();
}

void MainWindow::onNextMonth() {
    m_currentDate = m_currentDate.addMonths(1);
    updateMonthLabel();
    loadTransactionsForMonth();
}

void MainWindow::updateMonthLabel() {
    m_monthLabel->setText(m_currentDate.toString("MMMM yyyy"));
}

void MainWindow::loadTransactionsForMonth() {
    QDate firstDay(m_currentDate.year(), m_currentDate.month(), 1);
    QDate lastDay = firstDay.addMonths(1).addDays(-1);
    
    QString startDate = firstDay.toString("yyyy-MM-dd");
    QString endDate = lastDay.toString("yyyy-MM-dd");
    
    std::vector<Transaction> allTrans;
    if (m_currentAccountId == 0) {
        allTrans = m_db.getAllTransactions();
    } else {
        allTrans = m_db.getTransactionsByAccount(m_currentAccountId);
    }
    
    m_transactions.clear();
    for (const auto& t : allTrans) {
        QString tDate = QString::fromStdString(t.getDate());
        if (tDate >= startDate && tDate <= endDate) {
            m_transactions.push_back(t);
        }
    }
    
    refreshTransactions();
    refreshStatistics();
}

void MainWindow::onShowChart() {
    ChartTrackerDialog dialog(m_db, this);
    dialog.exec();
}