#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDate>

#include "../database/database.h"
#include "../models/transaction.h"
#include "../models/category.h"
#include "../models/account.h"
#include "../services/statistics.h"
#include "widgets/summarysidebar.h"

/**
 * @class MainWindow
 * @brief Main application window for the Finance Tracker
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onAddTransaction();
    void onEditTransaction();
    void onDeleteTransaction();
    void onManageCategories();
    void onManageAccounts();
    void onManageRecurring();
    void onManageSavingsGoals();
    void onManageTags();
    void onExportJson();
    void onImportJson();
    void onExportCsv();
    void onImportCsv();
    void onGeneratePdfReport();
    void onToggleTheme();
    void onSettings();
    void onShowChart();
    void onSearch();
    void onFilter();
    void onAccountChanged(int index);
    void onTransactionSelectionChanged();
    void onPrevMonth();
    void onNextMonth();
    void refreshData();
    void refreshTransactions();
    void refreshStatistics();

private:
    void setupUi();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void createConnections();
    void updateMonthLabel();
    void loadTransactionsForMonth();

    Database m_db;
    Statistics m_stats;
    std::vector<Transaction> m_transactions;
    std::vector<Category> m_categories;
    std::vector<Account> m_accounts;
    int m_currentAccountId;
    QDate m_currentDate;

    QTableWidget* m_transactionTable;
    QComboBox* m_accountCombo;
    QLineEdit* m_searchEdit;
    SummarySidebar* m_summarySidebar;
    QLabel* m_statusLabel;
    QLabel* m_monthLabel;
};

#endif // MAINWINDOW_H