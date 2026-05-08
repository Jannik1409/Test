# Troubleshooting Guide

```
╔═══════════════════════════════════════════════════════════╗
║                   TROUBLESHOOTING                          ║
╚═══════════════════════════════════════════════════════════╝
```

## Build Issues

### CMake Cannot Find Qt

```bash
# Find Qt installation
qmake -query QT_VERSION

# If not found, install Qt
# Ubuntu/Debian
sudo apt-get install qt5-default

# Set Qt path manually
export Qt5_DIR=/path/to/qt5
```

### CMake Cannot Find SQLite

```bash
# Ubuntu/Debian
sudo apt-get install libsqlite3-dev

# Verify installation
pkg-config --libs sqlite3
```

### Build Errors - Undefined Reference

```bash
# Clean rebuild
make clean
make
```

### Compiler Errors

```bash
# Check GCC version (need 9+)
gcc --version

# Check CMake version (need 3.16+)
cmake --version
```

## Runtime Issues

### Database Not Opening

```
Error: Failed to open database!
```

**Solutions:**
1. Check database file permissions
2. Ensure `data/` directory exists
3. Verify SQLite is properly linked

### Application Won't Start

```
Error: "Unable to find Qt platform plugin"
```

**Solutions:**
```bash
# Set Qt platform
export QT_QPA_PLATFORM=offscreen

# Or install platform plugins
sudo apt-get install qt5-platform-plugin-xcb
```

### Window Not Displaying

```bash
# Check display
echo $DISPLAY

# If empty, set display
export DISPLAY=:0
```

## Data Issues

### Transactions Not Showing

1. **Check month filter** - Use `< >` to navigate months
2. **Check account filter** - Ensure correct account selected
3. **Check date range** - Verify filter settings

### Import/Export Problems

**JSON Import Fails:**
- Check file format is valid JSON
- Verify required fields are present

**CSV Import Fails:**
- Ensure correct column order
- Check date format (YYYY-MM-DD)
- Verify amounts are numbers

### PDF Generation Fails

```bash
# Check file permissions
chmod 644 data/
```

## Performance Issues

### Slow with Large Database

1. **Create indexes** - Already included in schema
2. **Archive old data** - Move old transactions to archive
3. **Limit displayed records** - Use date filters

### Memory Leaks

Check logs for warnings:
```bash
# View application logs
cat logs/finance_tracker.log
```

## Common Error Messages

| Error | Cause | Solution |
|-------|-------|----------|
| `QApplication: no such file or directory` | Qt not installed | Install qt5-default |
| `sqlite3.h: No such file` | SQLite dev not installed | Install libsqlite3-dev |
| `No rule to make target` | Build system not configured | Run `cmake .` |
| `Permission denied` | No write access | Check file permissions |

## Debug Mode

Enable debug logging:
```cpp
// In your code
Logger::setLevel(Logger::DEBUG);
```

Check logs in:
- Console output
- `logs/app.log` (if configured)

## Getting Help

1. Check [README.md](../README.md)
2. Review [installation.md](installation.md)
3. Examine database schema in [database.md](database.md)
4. Check application logs

---

*For more help, open an issue on GitHub*