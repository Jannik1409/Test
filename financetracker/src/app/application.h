#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class MainWindow;

/**
 * @class Application
 * @brief Main application class
 */
class Application : public QApplication {
    Q_OBJECT

public:
    explicit Application(int& argc, char** argv);
    ~Application() = default;

    bool initialize();

private:
    MainWindow* m_mainWindow;
};

#endif // APPLICATION_H