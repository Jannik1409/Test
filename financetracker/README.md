# Finance Tracker

A production-ready C++ personal finance application for tracking income, expenses, budgets, and savings goals.

## Features

### Core Features
- **Transaction Management** - Add, edit, delete income and expense transactions
- **Category System** - Custom categories with budget limits
- **Multiple Accounts** - Support for checking, savings, credit card, cash, investment accounts
- **Monthly Summary** - Total income, expenses, and balance calculations
- **Search & Filter** - Find transactions by description or date range
- **Data Persistence** - SQLite database with automatic schema creation
- **JSON Import/Export** - Backup and restore your data

### Advanced Features
- **Recurring Transactions** - Auto-generate recurring entries (salary, rent, subscriptions)
- **Budget Limits per Category** - Set spending limits with visual warnings
- **Savings Goals** - Track progress toward financial targets
- **Tags** - Label transactions for flexible filtering
- **CSV Import** - Import from bank statement exports
- **PDF Reports** - Generate HTML-based financial reports
- **Theme Support** - Light/dark mode toggle
- **Auto-Backup** - Database backup capability

## Project Structure

```
finance_visualiser/
├── CMakeLists.txt         # CMake build configuration
├── Makefile               # Build wrapper
├── README.md              # This file
├── SPEC.md                # Detailed specification
├── include/               # External headers (nlohmann/json)
├── src/
│   ├── main.cpp           # Application entry point
│   ├── app/               # Application class
│   ├── models/            # Data models (Transaction, Category, Account, etc.)
│   ├── database/          # SQLite database wrapper
│   ├── services/         # Business logic (Statistics, JSON, CSV, PDF)
│   ├── ui/                # Qt GUI components
│   └── utils/             # Utilities (DateUtils, Validator, Logger)
├── tests/                 # Unit tests
└── data/                  # Database storage
```

## Requirements

- **C++17** compatible compiler
- **SQLite3** development library
- **CMake 3.16+**
- **Qt5** (optional - for GUI, console version works without it)

## Building

### Quick Build
```bash
make build
```

### Debug Build
```bash
make debug
```

### Run
```bash
make run
```

### Clean
```bash
make clean
```

## Usage

The application runs as a console demo showing:
- Database initialization with default categories and accounts
- Transaction creation and retrieval
- Monthly financial summary (income, expenses, balance)

Sample output:
```
Database opened successfully!
Categories: 10
  - Food (Expense)
  - Transportation (Expense)
  - Salary (Income)
Created transaction with ID: 1
Total transactions: 1

=== Financial Summary ===
Total Income: $1500
Total Expenses: $0
Balance: $1500
```

## Database Schema

### Tables
- `accounts` - Financial accounts
- `categories` - Transaction categories with budget limits
- `transactions` - Income/expense records
- `recurring` - Recurring transaction templates
- `savings_goals` - Savings targets
- `tags` - Transaction labels

## JSON Export/Import

Export data:
```cpp
JsonHandler::exportToFile("backup.json", transactions, categories, accounts, goals, tags);
```

Import data:
```cpp
JsonHandler::importFromFile("backup.json", transactions, categories, accounts, goals, tags);
```

## Statistics API

```cpp
Statistics stats;
double income = stats.calculateTotalIncome(transactions);
double expenses = stats.calculateTotalExpenses(transactions);
double balance = stats.calculateBalance(transactions);
auto monthlyData = stats.getMonthlyData(transactions);
auto spendingByCategory = stats.getSpendingByCategory(transactions);
```

## Testing

```bash
make test
```

## License

MIT License