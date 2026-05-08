cmake_minimum_required(VERSION 3.16)
project(FinanceTracker VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(SQLite3 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES
    src/main.cpp
    src/models/account.cpp
    src/models/category.cpp
    src/models/transaction.cpp
    src/models/recurring.cpp
    src/models/savings_goal.cpp
    src/models/tag.cpp
    src/database/database.cpp
    src/services/statistics.cpp
    src/services/json_handler.cpp
    src/services/csv_importer.cpp
    src/services/pdf_generator.cpp
    src/services/theme_manager.cpp
    src/utils/dateutils.cpp
    src/utils/validator.cpp
    src/utils/logger.cpp
    src/ui/mainwindow.cpp
    src/ui/dialogs/addtransactiondialog.cpp
    src/ui/dialogs/categorydialog.cpp
    src/ui/dialogs/accountdialog.cpp
    src/ui/dialogs/recurringdialog.cpp
    src/ui/dialogs/savingsgoaldialog.cpp
    src/ui/dialogs/settingsdialog.cpp
    src/ui/dialogs/filterdialog.cpp
    src/ui/dialogs/tagsdialog.cpp
    src/ui/dialogs/charttrackerdialog.cpp
    src/ui/widgets/summarysidebar.cpp
    src/ui/widgets/transactiontable.cpp
    src/ui/widgets/categorypiechart.cpp
    src/ui/widgets/budgetprogress.cpp
    src/ui/widgets/accountcard.cpp
)

set(HEADERS
    src/models/account.h
    src/models/category.h
    src/models/transaction.h
    src/models/recurring.h
    src/models/savings_goal.h
    src/models/tag.h
    src/database/database.h
    src/services/statistics.h
    src/services/json_handler.h
    src/services/csv_importer.h
    src/services/pdf_generator.h
    src/services/theme_manager.h
    src/utils/dateutils.h
    src/utils/validator.h
    src/utils/logger.h
    src/ui/mainwindow.h
    src/ui/dialogs/addtransactiondialog.h
    src/ui/dialogs/categorydialog.h
    src/ui/dialogs/accountdialog.h
    src/ui/dialogs/recurringdialog.h
    src/ui/dialogs/savingsgoaldialog.h
    src/ui/dialogs/settingsdialog.h
    src/ui/dialogs/filterdialog.h
    src/ui/dialogs/charttrackerdialog.h
    src/ui/widgets/summarysidebar.h
    src/ui/widgets/transactiontable.h
    src/ui/widgets/categorypiechart.h
    src/ui/widgets/budgetprogress.h
    src/ui/widgets/accountcard.h
)

add_executable(finance_tracker ${SOURCES} ${HEADERS})

target_link_libraries(finance_tracker
    Qt5::Core
    Qt5::Widgets
    SQLite::SQLite3
)

target_compile_definitions(finance_tracker PRIVATE QT_DISABLE_DEPRECATED_BEFORE=0x050F00)

install(TARGETS finance_tracker DESTINATION bin)