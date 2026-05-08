# Finance Tracker Makefile
# Wrapper around CMake build system

.PHONY: all clean build test install help

# Default target
all: build

# Create build directory and build
build:
	@mkdir -p build
	@cd build && cmake .. -DCMAKE_BUILD_TYPE=Release
	@cd build && make -j$(shell nproc)

# Build with debug symbols
debug:
	@mkdir -p build
	@cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug
	@cd build && make -j$(shell nproc)

# Run tests
test: build
	@cd build && ./run_tests

# Clean build artifacts
clean:
	@rm -rf build

# Install application
install: build
	@cd build && make install

# Run the application
run: build
	@cd build && ./finance_tracker

# Format code (requires clang-format)
format:
	@find src tests -name "*.cpp" -o -name "*.h" | xargs clang-format -i

# Show help
help:
	@echo "Finance Tracker Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  build    - Build the application (Release mode)"
	@echo "  debug    - Build with debug symbols"
	@echo "  test     - Run unit tests"
	@echo "  clean    - Remove build artifacts"
	@echo "  install  - Install the application"
	@echo "  run      - Build and run the application"
	@echo "  format   - Format source code (requires clang-format)"
	@echo "  help     - Show this help message"