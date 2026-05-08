# Database Schema

```
╔═══════════════════════════════════════════════════════════╗
║                    DATABASE SCHEMA                        ║
╚═══════════════════════════════════════════════════════════╝
```

## Entity Relationship Diagram

```
┌──────────────┐       ┌──────────────┐       ┌──────────────┐
│   Account    │       │  Category    │       │ Transaction  │
├──────────────┤       ├──────────────┤       ├──────────────┤
│ id           │       │ id           │       │ id           │
│ name         │       │ name         │──────▶│ account_id   │
│ type         │       │ type         │       │ category_id  │
│ balance      │       │ color        │       │ date         │
│ color        │       │ budget_limit │       │ amount       │
└──────────────┘       └──────────────┘       │ description  │
                                              │ type         │
                                              │ tags         │
                                              └──────────────┘
                                                    │
                                                    ▼
                                              ┌──────────────┐
                                              │    Tag       │
                                              ├──────────────┤
                                              │ id           │
                                              │ name         │
                                              └──────────────┘

      ┌──────────────┐
      │    Recurring │
      ├──────────────┤
      │ id           │
      │ account_id   │
      │ category_id  │
      │ amount       │
      │ description  │
      │ frequency    │
      │ start_date   │
      │ end_date     │
      │ active       │
      └──────────────┘

      ┌──────────────┐
      │ SavingsGoal  │
      ├──────────────┤
      │ id           │
      │ name         │
      │ target_amount│
      │ current_amount
      │ target_date  │
      └──────────────┘
```

## Tables

### accounts
```sql
CREATE TABLE accounts (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    type TEXT NOT NULL,
    balance REAL DEFAULT 0,
    color TEXT DEFAULT '#3498DB'
);
```

### categories
```sql
CREATE TABLE categories (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    type TEXT NOT NULL,
    color TEXT DEFAULT '#3498DB',
    budget_limit REAL DEFAULT 0
);
```

### transactions
```sql
CREATE TABLE transactions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    account_id INTEGER NOT NULL,
    category_id INTEGER NOT NULL,
    date TEXT NOT NULL,
    amount REAL NOT NULL,
    description TEXT,
    type TEXT NOT NULL,
    tags TEXT,
    FOREIGN KEY (account_id) REFERENCES accounts(id),
    FOREIGN KEY (category_id) REFERENCES categories(id)
);
```

### recurring
```sql
CREATE TABLE recurring (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    account_id INTEGER NOT NULL,
    category_id INTEGER NOT NULL,
    amount REAL NOT NULL,
    description TEXT,
    type TEXT NOT NULL,
    frequency TEXT NOT NULL,
    start_date TEXT NOT NULL,
    end_date TEXT,
    active INTEGER DEFAULT 1,
    FOREIGN KEY (account_id) REFERENCES accounts(id),
    FOREIGN KEY (category_id) REFERENCES categories(id)
);
```

### savings_goals
```sql
CREATE TABLE savings_goals (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    target_amount REAL NOT NULL,
    current_amount REAL DEFAULT 0,
    target_date TEXT
);
```

### tags
```sql
CREATE TABLE tags (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE
);
```

## Indexes

```sql
CREATE INDEX idx_transactions_date ON transactions(date);
CREATE INDEX idx_transactions_account ON transactions(account_id);
CREATE INDEX idx_transactions_category ON transactions(category_id);
```

---

*See [architecture.md](architecture.md) for code structure*