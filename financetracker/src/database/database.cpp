#include "database.h"
#include "../utils/logger.h"
#include <algorithm>
#include <sstream>
#include <cstring>

Database::Database(const std::string& dbPath)
    : m_db(nullptr), m_dbPath(dbPath), m_stmt(nullptr) {}

Database::~Database() {
    close();
}

bool Database::open() {
    if (m_db) return true;

    int result = sqlite3_open(m_dbPath.c_str(), &m_db);
    if (result != SQLITE_OK) {
        m_lastError = sqlite3_errmsg(m_db);
        Logger::error("Failed to open database: " + m_lastError);
        return false;
    }

    Logger::info("Database opened: " + m_dbPath);
    
    if (!createSchema()) return false;
    seedDefaultData();
    
    return true;
}

void Database::seedDefaultData() {
    auto accounts = getAllAccounts();
    if (accounts.empty()) {
        Account checking("Main Checking", "Checking", 0.0, "#3498DB");
        Account savings("Savings", "Savings", 0.0, "#2ECC71");
        createAccount(checking);
        createAccount(savings);
        Logger::info("Default accounts created");
    }
    
    auto categories = getAllCategories();
    if (categories.empty()) {
        std::vector<Category> defaultCategories = {
            {"Salary", Category::Type::Income, "#27AE60", 0},
            {"Freelance", Category::Type::Income, "#2ECC71", 0},
            {"Investments", Category::Type::Income, "#1ABC9C", 0},
            {"Other Income", Category::Type::Income, "#16A085", 0},
            {"Housing", Category::Type::Expense, "#E74C3C", 1500},
            {"Food & Dining", Category::Type::Expense, "#E67E22", 500},
            {"Transportation", Category::Type::Expense, "#F39C12", 300},
            {"Shopping", Category::Type::Expense, "#D35400", 400},
            {"Healthcare", Category::Type::Expense, "#C0392B", 200},
            {"Entertainment", Category::Type::Expense, "#9B59B6", 150},
            {"Utilities", Category::Type::Expense, "#3498DB", 200},
            {"Other Expense", Category::Type::Expense, "#95A5A6", 100}
        };
        
        for (const auto& cat : defaultCategories) {
            createCategory(cat);
        }
        Logger::info("Default categories created");
    }
}

void Database::close() {
    if (m_stmt) {
        sqlite3_finalize(m_stmt);
        m_stmt = nullptr;
    }
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
        Logger::info("Database closed");
    }
}

bool Database::isOpen() const {
    return m_db != nullptr;
}

bool Database::execute(const std::string& sql) {
    char* errorMsg = nullptr;
    int result = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &errorMsg);
    if (result != SQLITE_OK) {
        m_lastError = errorMsg ? errorMsg : "Unknown error";
        Logger::error("SQL Error: " + m_lastError);
        sqlite3_free(errorMsg);
        return false;
    }
    return true;
}

bool Database::prepareStatement(const std::string& sql) {
    resetStatement();
    int result = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &m_stmt, nullptr);
    if (result != SQLITE_OK) {
        m_lastError = sqlite3_errmsg(m_db);
        Logger::error("Failed to prepare statement: " + m_lastError);
        return false;
    }
    return true;
}

void Database::resetStatement() {
    if (m_stmt) {
        sqlite3_finalize(m_stmt);
        m_stmt = nullptr;
    }
}

bool Database::step() {
    if (!m_stmt) return false;
    int result = sqlite3_step(m_stmt);
    return result == SQLITE_ROW || result == SQLITE_DONE;
}

std::string Database::getColumnText(int col) const {
    if (!m_stmt) return "";
    const char* text = (const char*)sqlite3_column_text(m_stmt, col);
    return text ? text : "";
}

double Database::getColumnDouble(int col) const {
    if (!m_stmt) return 0.0;
    return sqlite3_column_double(m_stmt, col);
}

int Database::getColumnInt(int col) const {
    if (!m_stmt) return 0;
    return sqlite3_column_int(m_stmt, col);
}

bool Database::createSchema() {
    std::string sql = R"(
        CREATE TABLE IF NOT EXISTS accounts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            type TEXT NOT NULL,
            balance REAL DEFAULT 0,
            color TEXT DEFAULT '#3498DB',
            created_at TEXT DEFAULT (datetime('now'))
        );

        CREATE TABLE IF NOT EXISTS categories (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE,
            type TEXT NOT NULL,
            color TEXT DEFAULT '#3498DB',
            budget_limit REAL DEFAULT 0,
            created_at TEXT DEFAULT (datetime('now'))
        );

        CREATE TABLE IF NOT EXISTS transactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            account_id INTEGER NOT NULL,
            date TEXT NOT NULL,
            amount REAL NOT NULL,
            description TEXT,
            category_id INTEGER,
            type TEXT NOT NULL,
            tags TEXT,
            recurring_id INTEGER,
            created_at TEXT DEFAULT (datetime('now')),
            updated_at TEXT,
            deleted INTEGER DEFAULT 0,
            FOREIGN KEY (account_id) REFERENCES accounts(id),
            FOREIGN KEY (category_id) REFERENCES categories(id)
        );

        CREATE TABLE IF NOT EXISTS recurrings (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            category_id INTEGER NOT NULL,
            amount REAL NOT NULL,
            description TEXT,
            frequency TEXT NOT NULL,
            start_date TEXT NOT NULL,
            end_date TEXT,
            last_generated TEXT,
            FOREIGN KEY (category_id) REFERENCES categories(id)
        );

        CREATE TABLE IF NOT EXISTS savings_goals (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            target_amount REAL NOT NULL,
            current_amount REAL DEFAULT 0,
            target_date TEXT,
            color TEXT DEFAULT '#27AE60',
            created_at TEXT DEFAULT (datetime('now'))
        );

        CREATE TABLE IF NOT EXISTS tags (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE,
            color TEXT DEFAULT '#9B59B6',
            created_at TEXT DEFAULT (datetime('now'))
        );

        CREATE INDEX IF NOT EXISTS idx_transactions_date ON transactions(date);
        CREATE INDEX IF NOT EXISTS idx_transactions_account ON transactions(account_id);
        CREATE INDEX IF NOT EXISTS idx_transactions_category ON transactions(category_id);
    )";

    if (!execute(sql)) return false;
    return insertDefaultData();
}

bool Database::insertDefaultData() {
    std::string checkSql = "SELECT COUNT(*) FROM categories";
    if (!prepareStatement(checkSql)) return false;
    step();
    int count = getColumnInt(0);
    resetStatement();

    if (count > 0) return true;

    execute("INSERT INTO categories (name, type, color) VALUES ('Salary', 'Income', '#27AE60')");
    execute("INSERT INTO categories (name, type, color) VALUES ('Freelance', 'Income', '#2ECC71')");
    execute("INSERT INTO categories (name, type, color) VALUES ('Investments', 'Income', '#16A085')");
    execute("INSERT INTO categories (name, type, color, budget_limit) VALUES ('Food', 'Expense', '#E74C3C', 500)");
    execute("INSERT INTO categories (name, type, color, budget_limit) VALUES ('Transportation', 'Expense', '#3498DB', 200)");
    execute("INSERT INTO categories (name, type, color, budget_limit) VALUES ('Housing', 'Expense', '#9B59B6', 1500)");
    execute("INSERT INTO categories (name, type, color, budget_limit) VALUES ('Utilities', 'Expense', '#F39C12', 150)");
    execute("INSERT INTO categories (name, type, color, budget_limit) VALUES ('Entertainment', 'Expense', '#E91E63', 100)");
    execute("INSERT INTO categories (name, type, color, budget_limit) VALUES ('Shopping', 'Expense', '#00BCD4', 200)");
    execute("INSERT INTO categories (name, type, color, budget_limit) VALUES ('Healthcare', 'Expense', '#8BC34A', 100)");

    execute("INSERT INTO accounts (name, type, balance, color) VALUES ('Main Checking', 'Checking', 0, '#3498DB')");
    execute("INSERT INTO accounts (name, type, balance, color) VALUES ('Cash', 'Cash', 0, '#27AE60')");

    Logger::info("Default data inserted");
    return true;
}

// Account operations
int Database::createAccount(const Account& account) {
    std::string sql = "INSERT INTO accounts (name, type, balance, color) VALUES (?, ?, ?, ?)";
    if (!prepareStatement(sql)) return -1;
    sqlite3_bind_text(m_stmt, 1, account.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 2, account.getTypeString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(m_stmt, 3, account.getBalance());
    sqlite3_bind_text(m_stmt, 4, account.getColor().c_str(), -1, SQLITE_TRANSIENT);
    int rc = sqlite3_step(m_stmt);
    resetStatement();
    if (rc != SQLITE_DONE) return -1;
    return sqlite3_last_insert_rowid(m_db);
}

bool Database::updateAccount(const Account& account) {
    std::string sql = "UPDATE accounts SET name=?, type=?, balance=?, color=? WHERE id=?";
    if (!prepareStatement(sql)) return false;
    sqlite3_bind_text(m_stmt, 1, account.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 2, account.getTypeString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(m_stmt, 3, account.getBalance());
    sqlite3_bind_text(m_stmt, 4, account.getColor().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(m_stmt, 5, account.getId());
    bool result = step();
    resetStatement();
    return result;
}

bool Database::deleteAccount(int id) {
    std::string sql = "DELETE FROM accounts WHERE id=" + std::to_string(id);
    return execute(sql);
}

std::vector<Account> Database::getAllAccounts() const {
    std::vector<Account> result;
    const_cast<Database*>(this)->prepareStatement("SELECT id, name, type, balance, color, created_at FROM accounts ORDER BY name");
    while (true) {
        int rc = sqlite3_step(const_cast<Database*>(this)->m_stmt);
        if (rc != SQLITE_ROW) break;
        Account acc;
        acc.setId(getColumnInt(0));
        acc.setName(getColumnText(1));
        acc.setType(Account::typeFromString(getColumnText(2)));
        acc.setBalance(getColumnDouble(3));
        acc.setColor(getColumnText(4));
        acc.setCreatedAt(getColumnText(5));
        result.push_back(acc);
    }
    const_cast<Database*>(this)->resetStatement();
    return result;
}

Account Database::getAccountById(int id) const {
    Account acc;
    const_cast<Database*>(this)->prepareStatement("SELECT id, name, type, balance, color, created_at FROM accounts WHERE id=?");
    sqlite3_bind_int(const_cast<Database*>(this)->m_stmt, 1, id);
    if (const_cast<Database*>(this)->step()) {
        acc.setId(getColumnInt(0));
        acc.setName(getColumnText(1));
        acc.setType(Account::typeFromString(getColumnText(2)));
        acc.setBalance(getColumnDouble(3));
        acc.setColor(getColumnText(4));
        acc.setCreatedAt(getColumnText(5));
    }
    const_cast<Database*>(this)->resetStatement();
    return acc;
}

// Category operations
int Database::createCategory(const Category& cat) {
    std::string sql = "INSERT INTO categories (name, type, color, budget_limit) VALUES (?, ?, ?, ?)";
    if (!prepareStatement(sql)) return -1;
    sqlite3_bind_text(m_stmt, 1, cat.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 2, cat.getTypeString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 3, cat.getColor().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(m_stmt, 4, cat.getBudgetLimit());
    int rc = sqlite3_step(m_stmt);
    resetStatement();
    if (rc != SQLITE_DONE) return -1;
    return sqlite3_last_insert_rowid(m_db);
}

bool Database::updateCategory(const Category& cat) {
    std::string sql = "UPDATE categories SET name=?, type=?, color=?, budget_limit=? WHERE id=?";
    if (!prepareStatement(sql)) return false;
    sqlite3_bind_text(m_stmt, 1, cat.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 2, cat.getTypeString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 3, cat.getColor().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(m_stmt, 4, cat.getBudgetLimit());
    sqlite3_bind_int(m_stmt, 5, cat.getId());
    bool result = step();
    resetStatement();
    return result;
}

bool Database::deleteCategory(int id) {
    std::string sql = "DELETE FROM categories WHERE id=" + std::to_string(id);
    return execute(sql);
}

std::vector<Category> Database::getAllCategories() const {
    std::vector<Category> result;
    const_cast<Database*>(this)->prepareStatement("SELECT id, name, type, color, budget_limit, created_at FROM categories ORDER BY type, name");
    while (true) {
        int rc = sqlite3_step(const_cast<Database*>(this)->m_stmt);
        if (rc != SQLITE_ROW) break;
        Category cat;
        cat.setId(getColumnInt(0));
        cat.setName(getColumnText(1));
        cat.setType(Category::typeFromString(getColumnText(2)));
        cat.setColor(getColumnText(3));
        cat.setBudgetLimit(getColumnDouble(4));
        cat.setCreatedAt(getColumnText(5));
        result.push_back(cat);
    }
    const_cast<Database*>(this)->resetStatement();
    return result;
}

Category Database::getCategoryById(int id) const {
    Category cat;
    const_cast<Database*>(this)->prepareStatement("SELECT id, name, type, color, budget_limit, created_at FROM categories WHERE id=?");
    sqlite3_bind_int(const_cast<Database*>(this)->m_stmt, 1, id);
    if (const_cast<Database*>(this)->step()) {
        cat.setId(getColumnInt(0));
        cat.setName(getColumnText(1));
        cat.setType(Category::typeFromString(getColumnText(2)));
        cat.setColor(getColumnText(3));
        cat.setBudgetLimit(getColumnDouble(4));
        cat.setCreatedAt(getColumnText(5));
    }
    const_cast<Database*>(this)->resetStatement();
    return cat;
}

std::vector<Category> Database::getCategoriesByType(Category::Type type) const {
    std::vector<Category> result;
    const_cast<Database*>(this)->prepareStatement("SELECT id, name, type, color, budget_limit, created_at FROM categories WHERE type=? ORDER BY name");
    sqlite3_bind_text(const_cast<Database*>(this)->m_stmt, 1, Category::typeToString(type).c_str(), -1, SQLITE_TRANSIENT);
    while (true) {
        int rc = sqlite3_step(const_cast<Database*>(this)->m_stmt);
        if (rc != SQLITE_ROW) break;
        Category cat;
        cat.setId(getColumnInt(0));
        cat.setName(getColumnText(1));
        cat.setType(Category::typeFromString(getColumnText(2)));
        cat.setColor(getColumnText(3));
        cat.setBudgetLimit(getColumnDouble(4));
        cat.setCreatedAt(getColumnText(5));
        result.push_back(cat);
    }
    const_cast<Database*>(this)->resetStatement();
    return result;
}

// Transaction operations
int Database::createTransaction(const Transaction& trans) {
    std::string sql = R"(INSERT INTO transactions (account_id, date, amount, description, category_id, type, tags, recurring_id)
                        VALUES (?, ?, ?, ?, ?, ?, ?, ?))";
    if (!prepareStatement(sql)) return -1;
    sqlite3_bind_int(m_stmt, 1, trans.getAccountId());
    sqlite3_bind_text(m_stmt, 2, trans.getDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(m_stmt, 3, trans.getAmount());
    sqlite3_bind_text(m_stmt, 4, trans.getDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(m_stmt, 5, trans.getCategoryId());
    sqlite3_bind_text(m_stmt, 6, trans.getTypeString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 7, trans.getTags().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(m_stmt, 8, trans.getRecurringId());
    int rc = sqlite3_step(m_stmt);
    resetStatement();
    if (rc != SQLITE_DONE) return -1;
    return sqlite3_last_insert_rowid(m_db);
}

bool Database::updateTransaction(const Transaction& trans) {
    std::string sql = R"(UPDATE transactions SET account_id=?, date=?, amount=?, description=?,
                        category_id=?, type=?, tags=?, recurring_id=?, updated_at=datetime('now') WHERE id=?)";
    if (!prepareStatement(sql)) return false;
    sqlite3_bind_int(m_stmt, 1, trans.getAccountId());
    sqlite3_bind_text(m_stmt, 2, trans.getDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(m_stmt, 3, trans.getAmount());
    sqlite3_bind_text(m_stmt, 4, trans.getDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(m_stmt, 5, trans.getCategoryId());
    sqlite3_bind_text(m_stmt, 6, trans.getTypeString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 7, trans.getTags().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(m_stmt, 8, trans.getRecurringId());
    sqlite3_bind_int(m_stmt, 9, trans.getId());
    bool result = step();
    resetStatement();
    return result;
}

bool Database::deleteTransaction(int id) {
    std::string sql = "UPDATE transactions SET deleted=1, updated_at=datetime('now') WHERE id=" + std::to_string(id);
    return execute(sql);
}

std::vector<Transaction> Database::getAllTransactions() const {
    std::vector<Transaction> result;
    const_cast<Database*>(this)->prepareStatement(R"(SELECT id, account_id, date, amount, description, category_id, type, tags, recurring_id, created_at, updated_at, deleted
                        FROM transactions WHERE deleted=0 ORDER BY date DESC, id DESC)");
    while (true) {
        int rc = sqlite3_step(const_cast<Database*>(this)->m_stmt);
        if (rc != SQLITE_ROW) break;
        Transaction t;
        t.setId(getColumnInt(0));
        t.setAccountId(getColumnInt(1));
        t.setDate(getColumnText(2));
        t.setAmount(getColumnDouble(3));
        t.setDescription(getColumnText(4));
        t.setCategoryId(getColumnInt(5));
        t.setType(Transaction::typeFromString(getColumnText(6)));
        t.setTags(getColumnText(7));
        t.setRecurringId(getColumnInt(8));
        t.setCreatedAt(getColumnText(9));
        t.setUpdatedAt(getColumnText(10));
        t.setDeleted(getColumnInt(11) == 1);
        result.push_back(t);
    }
    const_cast<Database*>(this)->resetStatement();
    return result;
}

std::vector<Transaction> Database::getTransactionsByAccount(int accountId) const {
    std::vector<Transaction> result;
    const_cast<Database*>(this)->prepareStatement(R"(SELECT id, account_id, date, amount, description, category_id, type, tags, recurring_id, created_at, updated_at, deleted
                        FROM transactions WHERE account_id=? AND deleted=0 ORDER BY date DESC)");
    sqlite3_bind_int(const_cast<Database*>(this)->m_stmt, 1, accountId);
    while (true) {
        int rc = sqlite3_step(const_cast<Database*>(this)->m_stmt);
        if (rc != SQLITE_ROW) break;
        Transaction t;
        t.setId(getColumnInt(0));
        t.setAccountId(getColumnInt(1));
        t.setDate(getColumnText(2));
        t.setAmount(getColumnDouble(3));
        t.setDescription(getColumnText(4));
        t.setCategoryId(getColumnInt(5));
        t.setType(Transaction::typeFromString(getColumnText(6)));
        t.setTags(getColumnText(7));
        t.setRecurringId(getColumnInt(8));
        t.setCreatedAt(getColumnText(9));
        t.setUpdatedAt(getColumnText(10));
        t.setDeleted(getColumnInt(11) == 1);
        result.push_back(t);
    }
    const_cast<Database*>(this)->resetStatement();
    return result;
}

std::vector<Transaction> Database::getTransactionsByDateRange(const std::string& start, const std::string& end) const {
    std::vector<Transaction> result;
    const_cast<Database*>(this)->prepareStatement(R"(SELECT id, account_id, date, amount, description, category_id, type, tags, recurring_id, created_at, updated_at, deleted
                        FROM transactions WHERE date >= ? AND date <= ? AND deleted=0 ORDER BY date DESC)");
    sqlite3_bind_text(const_cast<Database*>(this)->m_stmt, 1, start.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(const_cast<Database*>(this)->m_stmt, 2, end.c_str(), -1, SQLITE_TRANSIENT);
    while (true) {
        int rc = sqlite3_step(const_cast<Database*>(this)->m_stmt);
        if (rc != SQLITE_ROW) break;
        Transaction t;
        t.setId(getColumnInt(0));
        t.setAccountId(getColumnInt(1));
        t.setDate(getColumnText(2));
        t.setAmount(getColumnDouble(3));
        t.setDescription(getColumnText(4));
        t.setCategoryId(getColumnInt(5));
        t.setType(Transaction::typeFromString(getColumnText(6)));
        t.setTags(getColumnText(7));
        t.setRecurringId(getColumnInt(8));
        t.setCreatedAt(getColumnText(9));
        t.setUpdatedAt(getColumnText(10));
        t.setDeleted(getColumnInt(11) == 1);
        result.push_back(t);
    }
    const_cast<Database*>(this)->resetStatement();
    return result;
}

std::vector<Transaction> Database::searchTransactions(const std::string& query) const {
    std::vector<Transaction> result;
    const_cast<Database*>(this)->prepareStatement(R"(SELECT id, account_id, date, amount, description, category_id, type, tags, recurring_id, created_at, updated_at, deleted
                        FROM transactions WHERE deleted=0 AND (description LIKE ? OR tags LIKE ?) ORDER BY date DESC)");
    std::string searchPattern = "%" + query + "%";
    sqlite3_bind_text(const_cast<Database*>(this)->m_stmt, 1, searchPattern.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(const_cast<Database*>(this)->m_stmt, 2, searchPattern.c_str(), -1, SQLITE_TRANSIENT);
    while (true) {
        int rc = sqlite3_step(const_cast<Database*>(this)->m_stmt);
        if (rc != SQLITE_ROW) break;
        Transaction t;
        t.setId(getColumnInt(0));
        t.setAccountId(getColumnInt(1));
        t.setDate(getColumnText(2));
        t.setAmount(getColumnDouble(3));
        t.setDescription(getColumnText(4));
        t.setCategoryId(getColumnInt(5));
        t.setType(Transaction::typeFromString(getColumnText(6)));
        t.setTags(getColumnText(7));
        t.setRecurringId(getColumnInt(8));
        t.setCreatedAt(getColumnText(9));
        t.setUpdatedAt(getColumnText(10));
        t.setDeleted(getColumnInt(11) == 1);
        result.push_back(t);
    }
    const_cast<Database*>(this)->resetStatement();
    return result;
}

Transaction Database::getTransactionById(int id) const {
    Transaction t;
    const_cast<Database*>(this)->prepareStatement(R"(SELECT id, account_id, date, amount, description, category_id, type, tags, recurring_id, created_at, updated_at, deleted
                        FROM transactions WHERE id=?)");
    sqlite3_bind_int(const_cast<Database*>(this)->m_stmt, 1, id);
    if (const_cast<Database*>(this)->step()) {
        t.setId(getColumnInt(0));
        t.setAccountId(getColumnInt(1));
        t.setDate(getColumnText(2));
        t.setAmount(getColumnDouble(3));
        t.setDescription(getColumnText(4));
        t.setCategoryId(getColumnInt(5));
        t.setType(Transaction::typeFromString(getColumnText(6)));
        t.setTags(getColumnText(7));
        t.setRecurringId(getColumnInt(8));
        t.setCreatedAt(getColumnText(9));
        t.setUpdatedAt(getColumnText(10));
        t.setDeleted(getColumnInt(11) == 1);
    }
    const_cast<Database*>(this)->resetStatement();
    return t;
}

// Recurring operations
int Database::createRecurring(const Recurring& rec) {
    std::string sql = "INSERT INTO recurrings (category_id, amount, description, frequency, start_date, end_date) VALUES (?, ?, ?, ?, ?, ?)";
    if (!prepareStatement(sql)) return -1;
    sqlite3_bind_int(m_stmt, 1, rec.getCategoryId());
    sqlite3_bind_double(m_stmt, 2, rec.getAmount());
    sqlite3_bind_text(m_stmt, 3, rec.getDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 4, rec.getFrequencyString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 5, rec.getStartDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 6, rec.getEndDate().c_str(), -1, SQLITE_TRANSIENT);
    if (!step()) { resetStatement(); return -1; }
    resetStatement();
    return sqlite3_last_insert_rowid(m_db);
}

bool Database::updateRecurring(const Recurring& rec) {
    std::string sql = "UPDATE recurrings SET category_id=?, amount=?, description=?, frequency=?, start_date=?, end_date=?, last_generated=? WHERE id=?";
    if (!prepareStatement(sql)) return false;
    sqlite3_bind_int(m_stmt, 1, rec.getCategoryId());
    sqlite3_bind_double(m_stmt, 2, rec.getAmount());
    sqlite3_bind_text(m_stmt, 3, rec.getDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 4, rec.getFrequencyString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 5, rec.getStartDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 6, rec.getEndDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 7, rec.getLastGenerated().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(m_stmt, 8, rec.getId());
    bool result = step();
    resetStatement();
    return result;
}

bool Database::deleteRecurring(int id) {
    std::string sql = "DELETE FROM recurrings WHERE id=" + std::to_string(id);
    return execute(sql);
}

std::vector<Recurring> Database::getAllRecurrings() const {
    std::vector<Recurring> result;
    const_cast<Database*>(this)->prepareStatement("SELECT id, category_id, amount, description, frequency, start_date, end_date, last_generated FROM recurrings ORDER BY id");
    while (true) {
        int rc = sqlite3_step(const_cast<Database*>(this)->m_stmt);
        if (rc != SQLITE_ROW) break;
        Recurring r;
        r.setId(getColumnInt(0));
        r.setCategoryId(getColumnInt(1));
        r.setAmount(getColumnDouble(2));
        r.setDescription(getColumnText(3));
        r.setFrequency(Recurring::frequencyFromString(getColumnText(4)));
        r.setStartDate(getColumnText(5));
        r.setEndDate(getColumnText(6));
        r.setLastGenerated(getColumnText(7));
        result.push_back(r);
    }
    const_cast<Database*>(this)->resetStatement();
    return result;
}

std::vector<Recurring> Database::getActiveRecurrings() const {
    std::vector<Recurring> result;
    const_cast<Database*>(this)->prepareStatement(R"(SELECT id, category_id, amount, description, frequency, start_date, end_date, last_generated
                        FROM recurrings WHERE (end_date IS NULL OR end_date >= date('now')) ORDER BY id)");
    while (true) {
        int rc = sqlite3_step(const_cast<Database*>(this)->m_stmt);
        if (rc != SQLITE_ROW) break;
        Recurring r;
        r.setId(getColumnInt(0));
        r.setCategoryId(getColumnInt(1));
        r.setAmount(getColumnDouble(2));
        r.setDescription(getColumnText(3));
        r.setFrequency(Recurring::frequencyFromString(getColumnText(4)));
        r.setStartDate(getColumnText(5));
        r.setEndDate(getColumnText(6));
        r.setLastGenerated(getColumnText(7));
        result.push_back(r);
    }
    const_cast<Database*>(this)->resetStatement();
    return result;
}

// Savings Goal operations
int Database::createSavingsGoal(const SavingsGoal& goal) {
    std::string sql = "INSERT INTO savings_goals (name, target_amount, current_amount, target_date, color) VALUES (?, ?, ?, ?, ?)";
    if (!prepareStatement(sql)) return -1;
    sqlite3_bind_text(m_stmt, 1, goal.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(m_stmt, 2, goal.getTargetAmount());
    sqlite3_bind_double(m_stmt, 3, goal.getCurrentAmount());
    sqlite3_bind_text(m_stmt, 4, goal.getTargetDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 5, goal.getColor().c_str(), -1, SQLITE_TRANSIENT);
    if (!step()) { resetStatement(); return -1; }
    resetStatement();
    return sqlite3_last_insert_rowid(m_db);
}

bool Database::updateSavingsGoal(const SavingsGoal& goal) {
    std::string sql = "UPDATE savings_goals SET name=?, target_amount=?, current_amount=?, target_date=?, color=? WHERE id=?";
    if (!prepareStatement(sql)) return false;
    sqlite3_bind_text(m_stmt, 1, goal.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(m_stmt, 2, goal.getTargetAmount());
    sqlite3_bind_double(m_stmt, 3, goal.getCurrentAmount());
    sqlite3_bind_text(m_stmt, 4, goal.getTargetDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 5, goal.getColor().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(m_stmt, 6, goal.getId());
    bool result = step();
    resetStatement();
    return result;
}

bool Database::deleteSavingsGoal(int id) {
    std::string sql = "DELETE FROM savings_goals WHERE id=" + std::to_string(id);
    return execute(sql);
}

std::vector<SavingsGoal> Database::getAllSavingsGoals() const {
    std::vector<SavingsGoal> result;
    const_cast<Database*>(this)->prepareStatement("SELECT id, name, target_amount, current_amount, target_date, color, created_at FROM savings_goals ORDER BY id");
    while (true) {
        int rc = sqlite3_step(const_cast<Database*>(this)->m_stmt);
        if (rc != SQLITE_ROW) break;
        SavingsGoal g;
        g.setId(getColumnInt(0));
        g.setName(getColumnText(1));
        g.setTargetAmount(getColumnDouble(2));
        g.setCurrentAmount(getColumnDouble(3));
        g.setTargetDate(getColumnText(4));
        g.setColor(getColumnText(5));
        g.setCreatedAt(getColumnText(6));
        result.push_back(g);
    }
    const_cast<Database*>(this)->resetStatement();
    return result;
}

SavingsGoal Database::getSavingsGoalById(int id) const {
    SavingsGoal g;
    const_cast<Database*>(this)->prepareStatement("SELECT id, name, target_amount, current_amount, target_date, color, created_at FROM savings_goals WHERE id=?");
    sqlite3_bind_int(const_cast<Database*>(this)->m_stmt, 1, id);
    if (const_cast<Database*>(this)->step()) {
        g.setId(getColumnInt(0));
        g.setName(getColumnText(1));
        g.setTargetAmount(getColumnDouble(2));
        g.setCurrentAmount(getColumnDouble(3));
        g.setTargetDate(getColumnText(4));
        g.setColor(getColumnText(5));
        g.setCreatedAt(getColumnText(6));
    }
    const_cast<Database*>(this)->resetStatement();
    return g;
}

// Tag operations
int Database::createTag(const Tag& tag) {
    std::string sql = "INSERT INTO tags (name, color) VALUES (?, ?)";
    if (!prepareStatement(sql)) return -1;
    sqlite3_bind_text(m_stmt, 1, tag.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(m_stmt, 2, tag.getColor().c_str(), -1, SQLITE_TRANSIENT);
    if (!step()) { resetStatement(); return -1; }
    resetStatement();
    return sqlite3_last_insert_rowid(m_db);
}

bool Database::deleteTag(int id) {
    std::string sql = "DELETE FROM tags WHERE id=" + std::to_string(id);
    return execute(sql);
}

std::vector<Tag> Database::getAllTags() const {
    std::vector<Tag> result;
    const_cast<Database*>(this)->prepareStatement("SELECT id, name, color, created_at FROM tags ORDER BY name");
    while (true) {
        int rc = sqlite3_step(const_cast<Database*>(this)->m_stmt);
        if (rc != SQLITE_ROW) break;
        Tag t;
        t.setId(getColumnInt(0));
        t.setName(getColumnText(1));
        t.setColor(getColumnText(2));
        t.setCreatedAt(getColumnText(3));
        result.push_back(t);
    }
    const_cast<Database*>(this)->resetStatement();
    return result;
}

// Utility
bool Database::backup(const std::string& backupPath) const {
    sqlite3* backupDb = nullptr;
    if (sqlite3_open(backupPath.c_str(), &backupDb) != SQLITE_OK) {
        return false;
    }
    sqlite3_backup* backup = sqlite3_backup_init(backupDb, "main", m_db, "main");
    if (backup) {
        sqlite3_backup_step(backup, -1);
        sqlite3_backup_finish(backup);
    }
    sqlite3_close(backupDb);
    return true;
}

std::string Database::getLastError() const {
    return m_lastError;
}