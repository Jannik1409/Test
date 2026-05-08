# Features Overview

```
╔═══════════════════════════════════════════════════════════╗
║                      FEATURES GUIDE                       ║
╚═══════════════════════════════════════════════════════════╝
```

## Core Features

### 💰 Transaction Management

| Feature | Description |
|---------|-------------|
| Add Transaction | Create income/expense with date, amount, description |
| Edit Transaction | Modify existing transactions |
| Delete Transaction | Remove transactions |
| Search | Find transactions by description |
| Filter | Filter by date range, type, amount |

### 🏦 Account Management

- **Multiple Accounts**: Checking, Savings, Credit Card, Cash, Investment
- **Account Colors**: Visual distinction with custom colors
- **Balance Tracking**: Per-account balance calculation
- **All Accounts View**: Combined view across all accounts

### 📁 Categories

- **Income Categories**: Salary, Freelance, Investments, etc.
- **Expense Categories**: Food, Transport, Housing, etc.
- **Budget Limits**: Set spending limits per category
- **Visual Colors**: Color-coded categories

### 🔄 Recurring Transactions

```
┌─────────────────────────────────────────────────┐
│  Recurring Transaction Types                    │
│  ───────────────────────────────────────────    │
│  • Daily      • Weekly                          │
│  • Bi-weekly  • Monthly                         │
│  • Quarterly  • Yearly                          │
└─────────────────────────────────────────────────┘
```

- Automatic transaction generation
- Subscription tracking
- Bill reminders

### 🎯 Savings Goals

- Set target amount
- Track progress
- Target date
- Visual progress indicator

### 🏷️ Tags

- Add custom tags to transactions
- Search by tags
- Organize expenses

## Import/Export Features

### 📤 Export Options

| Format   | Description               |
|----------|---------------------------|
| **JSON** | Full backup with all data |
| **CSV**  | Spreadsheet-compatible    |
| **PDF**  | Formatted reports         |

### 📥 Import Options

| Format   | Description            |
|----------|------------------------|
| **JSON** | Restore from backup    |
| **CSV**  | Import bank statements |

### PDF Reports

- Monthly financial summary
- Category breakdown
- Date range selection
- Professional formatting

## UI/Theme Features

### 🎨 Theme Support

- **Light Theme**: Clean, bright interface
- **Dark Theme**: Eye-friendly dark mode
- **Quick Toggle**: Ctrl+T to switch

### 📊 Summary Sidebar

- Current month income
- Current month expenses
- Current balance
- Visual indicators

### 📅 Month Navigation

- Navigate to previous/next months
- Month/year display
- Automatic filtering

### 🔍 Search & Filter

- Real-time search
- Date range filtering
- Type filtering (Income/Expense)
- Amount range filtering

## Data Management

### 💾 Automatic Storage

- SQLite database
- Auto-save on changes
- Schema migrations

### 🔒 Backup Features

- Manual backup
- Export to JSON
- Export to CSV
- Database backup

---

*See [architecture.md](architecture.md) for technical details*