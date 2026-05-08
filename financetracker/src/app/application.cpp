#include "application.h"
#include "../ui/mainwindow.h"
#include "../utils/logger.h"
#include "../services/theme_manager.h"

Application::Application(int& argc, char** argv)
    : QApplication(argc, argv), m_mainWindow(nullptr) {}

bool Application::initialize() {
    Logger::init("finance_tracker.log");
    ThemeManager::loadSettings();
    
    m_mainWindow = new MainWindow();
    m_mainWindow->show();
    
    return true;
}