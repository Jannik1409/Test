# Installation Guide

```
╔═══════════════════════════════════════════════════════════╗
║                   INSTALLATION GUIDE                      ║
╚═══════════════════════════════════════════════════════════╝
```

## System Requirements

| Requirement  | Version             | Notes           |
|--------------|---------------------|-----------------|
| **OS**       | Linux/Unix          | Tested on Linux |
| **Compiler** | GCC 9+ or Clang 10+ | C++17 required  |
| **CMake**    | 3.16+               | Build system    |
| **Qt**       | 5.15+               | GUI framework   |
| **SQLite**   | 3.x                 | Database        |

## Quick Install

### 1. Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential cmake qt5-default libsqlite3-dev
```

**Fedora/RHEL:**
```bash
sudo dnf install gcc-g++ cmake qt5-qtbase sqlite-devel
```

**Arch Linux:**
```bash
sudo pacman -S base-devel cmake qt5-base sqlite
```

### 2. Build the Project

```bash
# Clone and navigate to project
cd finance_visualiser

# Build using Makefile
make clean
make

# Or use CMake directly
mkdir build && cd build
cmake ..
make
```

### 3. Run the Application

```bash
./build/finance_tracker
```

## Project Structure

```
finance_visualiser/
├── CMakeLists.txt      # CMake configuration
├── Makefile            # Build wrapper
├── src/
│   ├── main.cpp       # Entry point
│   ├── models/        # Data models
│   ├── database/      # SQLite wrapper
│   ├── services/      # Business logic
│   ├── ui/            # Qt GUI
│   └── utils/         # Utilities
├── tests/             # Unit tests
├── docs/              # Documentation
└── data/              # Database storage
```

## Database Setup

The database is automatically created on first run:

- **Location**: `data/finance.db`
- **Schema**: Auto-migrated on startup
- **Default data**: Sample categories and accounts

## Troubleshooting

### Qt Not Found
```bash
# Find Qt installation
qmake -query QT_VERSION
```

### SQLite Not Found
```bash
# Ubuntu
sudo apt-get install libsqlite3-dev

# Verify
pkg-config --libs sqlite3
```

### Build Errors
```bash
# Clean rebuild
make clean
make
```

---

*See [README.md](../README.md) for more details*