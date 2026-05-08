#include <QApplication>
#include <QDebug>

#include "ui/mainwindow.h"
#include "utils/logger.h"
#include "services/theme_manager.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    Logger::init();
    Logger::info("Application starting...");
    
    ThemeManager::loadSettings();
    
    MainWindow window;
    window.show();
    
    return app.exec();
}