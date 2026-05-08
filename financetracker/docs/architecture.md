# Architecture Guide

```
╔═══════════════════════════════════════════════════════════╗
║                   ARCHITECTURE GUIDE                      ║
╚═══════════════════════════════════════════════════════════╝
```

## High-Level Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                     Finance Tracker                         │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────┐     ┌─────────────┐     ┌─────────────┐    │
│  │   Qt GUI    │───▶│  Services   │───▶│  Database   │    │
│  │  (MainWindow│     │(Statistics) │     │  (SQLite)   │    │
│  │  Dialogs)   │     │(JSON,CSV)   │     │             │    │
│  └─────────────┘     └─────────────┘     └─────────────┘    │
│         │                   │                   │           │
│         ▼                   ▼                   ▼           │
│  ┌─────────────┐     ┌─────────────┐     ┌─────────────┐    │
│  │   Models    │     │   Utils     │     │    Data     │    │
│  │(Transaction)│     │(DateUtils)  │     │   files     │    │
│  │(Account)    │     │(Validator)  │     │   (.db)     │    │
│  │(Category)   │     │(Logger)     │     │             │    │
│  └─────────────┘     └─────────────┘     └─────────────┘    │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

## Directory Structure

```
src/
├── main.cpp              # Application entry point
│
├── models/               # Data models (POJOs)
│   ├── account.h/cpp
│   ├── category.h/cpp
│   ├── transaction.h/cpp
│   ├── recurring.h/cpp
│   ├── savings_goal.h/cpp
│   └── tag.h/cpp
│
├── database/             # SQLite wrapper
│   ├── database.h
│   └── database.cpp
│
├── services/             # Business logic
│   ├── statistics.h/cpp # Financial calculations
│   ├── json_handler.h/cpp   # Import/Export JSON
│   ├── csv_importer.h/cpp   # CSV import
│   ├── pdf_generator.h/cpp   # PDF reports
│   └── theme_manager.h/cpp   # Theme handling
│
├── ui/                  # Qt GUI layer
│   ├── mainwindow.h/cpp
│   ├── dialogs/
│   │   ├── addtransactiondialog.h/cpp
│   │   ├── accountdialog.h/cpp
│   │   ├── categorydialog.h/cpp
│   │   └── ...
│   └── widgets/
│       ├── summarysidebar.h/cpp
│       └── ...
│
└── utils/               # Utility classes
    ├── dateutils.h/cpp
    ├── validator.h/cpp
    └── logger.h/cpp
```

## Layer Responsibilities

### Models Layer
- Pure data classes (POCOs)
- Getters/setters for properties
- No business logic
- Serializable to/from database

### Database Layer
- SQLite connection management
- CRUD operations for all entities
- Schema creation/migration
- Query building and execution

### Services Layer
- Business logic implementation
- Data transformation
- Statistics calculations
- Import/export operations

### UI Layer
- Qt widget implementation
- User interaction handling
- Data display
- Dialog management

### Utils Layer
- Date/time utilities
- Input validation
- Logging functionality
- Helper functions

## Design Patterns

### MVC Pattern
```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│    View     │◀───│ Controller  │───▶│    Model    │
│   (Qt UI)   │     │ (Services)  │     │ (Database)  │
└─────────────┘     └─────────────┘     └─────────────┘
```

### Repository Pattern
- Database class acts as repository
- Single point for data access
- Entity-based operations

### Factory Pattern
- Used for creating dialogs
- Category type conversion

---

*See [database.md](database.md) for schema details*