# UI Guide

```
╔═══════════════════════════════════════════════════════════╗
║                     UI GUIDE                              ║
╚═══════════════════════════════════════════════════════════╝
```

## Main Window Layout

```
┌────────────────────────────────────────────────────────────────────┐
│  Finance Tracker                                    [_][□][X]     │
├────────────────────────────────────────────────────────────────────┤
│  File   Transaction   Manage   View   Help                        │
├────────────────────────────────────────────────────────────────────┤
│  ◀ April 2026 ▶   [Account: All ▼]  [🔍 Search...] [Filter]       │
├──────────────────────┬─────────────────────────────────────────────┤
│                      │                                             │
│  ┌────────────────┐  │  ┌───────┬─────────┬─────────┬──────┬─────┐│
│  │ 💰 SUMMARY     │  │  │ Date  │Desc     │Category │Type  │Amt  ││
│  ├────────────────┤  │  ├───────┼─────────┼─────────┼──────┼─────┤│
│  │ Income:        │  │  │       │         │         │      │     ││
│  │   $5,000.00    │  │  │       │         │         │      │     ││
│  │                │  │  │       │         │         │      │     ││
│  │ Expenses:      │  │  │       │         │         │      │     ││
│  │   $2,350.00    │  │  │       │         │         │      │     ││
│  │                │  │  │       │         │         │      │     ││
│  │ ─────────────  │  │  │       │         │         │      │     ││
│  │                │  │  │       │         │         │      │     ││
│  │ Balance:       │  │  │       │         │         │      │     ││
│  │   $2,650.00    │  │  │       │         │         │      │     ││
│  └────────────────┘  │  └───────┴─────────┴─────────┴──────┴─────┘│
│                      │                                             │
│       25%            │              75%                            │
└──────────────────────┴─────────────────────────────────────────────┘
│  Ready | Records: 15                                               │
└────────────────────────────────────────────────────────────────────┘
```

## Left Panel - Summary

| Element | Description |
|---------|-------------|
| Month Navigation | `< Month Year >` buttons |
| Income | Total income for current month (green) |
| Expenses | Total expenses for current month (red) |
| Balance | Income - Expenses |

## Right Panel - Transactions

| Column | Description |
|--------|-------------|
| Date | Transaction date (YYYY-MM-DD) |
| Description | What the transaction was for |
| Category | Category name |
| Type | Income / Expense |
| Amount | Transaction amount |
| Tags | Associated tags |

## Toolbar Features

| Control | Function |
|---------|----------|
| `< >` | Navigate months |
| Account dropdown | Filter by account or show all |
| Search | Search transactions by description |
| Filter | Advanced filtering dialog |

## Menu Bar

### File Menu
- **Export to JSON** - Save all data as JSON
- **Import from JSON** - Load data from JSON
- **Export to CSV** - Export transactions as CSV
- **Import from CSV** - Import transactions from CSV
- **Generate PDF Report** - Create PDF summary
- **Exit** - Close application

### Transaction Menu
- **Add Transaction** - Create new transaction
- **Edit Transaction** - Modify selected transaction
- **Delete Transaction** - Remove selected transaction

### Manage Menu
- **Categories** - Manage expense/income categories
- **Accounts** - Manage bank accounts
- **Recurring** - Manage recurring transactions
- **Savings Goals** - Track savings targets
- **Tags** - Manage transaction tags

### View Menu
- **Toggle Theme** - Switch between light/dark
- **Settings** - Open settings dialog

## Dialogs

### Add Transaction Dialog
- Date picker
- Amount input
- Description field
- Income/Expense radio buttons
- Category dropdown
- Account dropdown
- Tags field

### Filter Dialog
- Date range picker
- Category filter
- Type filter (All/Income/Expense)
- Amount range (min/max)

### Settings Dialog
- Theme selection (Light/Dark)
- Currency selection
- Date format
- Notifications toggle
- Backup path

---

*See [troubleshooting.md](troubleshooting.md) for issues*