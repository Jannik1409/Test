#include "settingsdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>

#include "../../services/theme_manager.h"
#include "../../../include/nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Settings");
    resize(500, 400);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QHBoxLayout* themeLayout = new QHBoxLayout();
    themeLayout->addWidget(new QLabel("Theme:"));
    m_themeCombo = new QComboBox(this);
    m_themeCombo->addItem("Light");
    m_themeCombo->addItem("Dark");
    themeLayout->addWidget(m_themeCombo);
    mainLayout->addLayout(themeLayout);
    
    QHBoxLayout* currencyLayout = new QHBoxLayout();
    currencyLayout->addWidget(new QLabel("Currency:"));
    m_currencyCombo = new QComboBox(this);
    m_currencyCombo->addItem("USD ($)");
    m_currencyCombo->addItem("EUR (€)");
    m_currencyCombo->addItem("GBP (£)");
    m_currencyCombo->addItem("JPY (¥)");
    currencyLayout->addWidget(m_currencyCombo);
    mainLayout->addLayout(currencyLayout);
    
    QHBoxLayout* dateFormatLayout = new QHBoxLayout();
    dateFormatLayout->addWidget(new QLabel("Date Format:"));
    m_dateFormatCombo = new QComboBox(this);
    m_dateFormatCombo->addItem("YYYY-MM-DD");
    m_dateFormatCombo->addItem("DD/MM/YYYY");
    m_dateFormatCombo->addItem("MM/DD/YYYY");
    dateFormatLayout->addWidget(m_dateFormatCombo);
    mainLayout->addLayout(dateFormatLayout);
    
    m_notificationsCheck = new QCheckBox("Enable notifications", this);
    mainLayout->addWidget(m_notificationsCheck);
    
    QHBoxLayout* backupLayout = new QHBoxLayout();
    backupLayout->addWidget(new QLabel("Backup Path:"));
    m_backupPathEdit = new QLineEdit(this);
    m_backupPathEdit->setPlaceholderText("data/finance.db");
    m_browseBtn = new QPushButton("Browse", this);
    backupLayout->addWidget(m_backupPathEdit);
    backupLayout->addWidget(m_browseBtn);
    mainLayout->addLayout(backupLayout);
    
    mainLayout->addStretch();
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_saveButton = new QPushButton("Save", this);
    m_cancelButton = new QPushButton("Cancel", this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_cancelButton);
    mainLayout->addLayout(buttonLayout);
    
    loadSettings();
    
    connect(m_saveButton, &QPushButton::clicked, this, &SettingsDialog::onSave);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_browseBtn, &QPushButton::clicked, this, &SettingsDialog::onBrowse);
    connect(m_themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &SettingsDialog::onThemeChanged);
}

void SettingsDialog::loadSettings() {
    std::ifstream file("data/settings.json");
    if (file.is_open()) {
        try {
            json j;
            file >> j;
            
            if (j.contains("theme")) {
                std::string theme = j["theme"];
                if (theme == "dark") m_themeCombo->setCurrentIndex(1);
                else if (theme == "system") m_themeCombo->setCurrentIndex(2);
                else m_themeCombo->setCurrentIndex(0);
            }
            
            if (j.contains("currency")) {
                std::string currency = j["currency"];
                if (currency == "EUR") m_currencyCombo->setCurrentIndex(1);
                else if (currency == "GBP") m_currencyCombo->setCurrentIndex(2);
                else if (currency == "JPY") m_currencyCombo->setCurrentIndex(3);
                else m_currencyCombo->setCurrentIndex(0);
            }
            
            if (j.contains("dateFormat")) {
                std::string format = j["dateFormat"];
                if (format == "DD/MM/YYYY") m_dateFormatCombo->setCurrentIndex(1);
                else if (format == "MM/DD/YYYY") m_dateFormatCombo->setCurrentIndex(2);
                else m_dateFormatCombo->setCurrentIndex(0);
            }
            
            if (j.contains("notifications")) {
                m_notificationsCheck->setChecked(j["notifications"]);
            }
            
            if (j.contains("backupPath")) {
                m_backupPathEdit->setText(QString::fromStdString(j["backupPath"]));
            }
        } catch (...) {
        }
        file.close();
    } else {
        m_themeCombo->setCurrentIndex(0);
        m_currencyCombo->setCurrentIndex(0);
        m_dateFormatCombo->setCurrentIndex(0);
        m_notificationsCheck->setChecked(true);
        m_backupPathEdit->setText("data/finance.db");
    }
}

void SettingsDialog::saveSettings() {
    json j;
    
    std::ifstream inputFile("data/settings.json");
    if (inputFile.is_open()) {
        try {
            inputFile >> j;
        } catch (...) {
            j = json::object();
        }
        inputFile.close();
    }
    
    QString themeStr = m_themeCombo->currentText();
    if (themeStr == "Dark") j["theme"] = "dark";
    else if (themeStr == "System") j["theme"] = "system";
    else j["theme"] = "light";
    
    QString currencyStr = m_currencyCombo->currentText();
    if (currencyStr.contains("EUR")) j["currency"] = "EUR";
    else if (currencyStr.contains("GBP")) j["currency"] = "GBP";
    else if (currencyStr.contains("JPY")) j["currency"] = "JPY";
    else j["currency"] = "USD";
    
    QString dateStr = m_dateFormatCombo->currentText();
    j["dateFormat"] = dateStr.toStdString();
    
    j["notifications"] = m_notificationsCheck->isChecked();
    
    j["backupPath"] = m_backupPathEdit->text().toStdString();
    
    std::ofstream outputFile("data/settings.json");
    if (outputFile.is_open()) {
        outputFile << j.dump(4);
        outputFile.close();
    }
    
    if (m_themeCombo->currentIndex() == 1) {
        ThemeManager::setTheme(ThemeManager::Theme::Dark);
    } else if (m_themeCombo->currentIndex() == 2) {
        ThemeManager::setTheme(ThemeManager::Theme::System);
    } else {
        ThemeManager::setTheme(ThemeManager::Theme::Light);
    }
    
    ThemeManager::setDateFormat(dateStr.toStdString());
}

void SettingsDialog::onSave() {
    saveSettings();
    QMessageBox::information(this, "Settings", "Settings saved successfully!");
    accept();
}

void SettingsDialog::onThemeChanged(int index) {
}

void SettingsDialog::onBrowse() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Backup Directory");
    if (!dir.isEmpty()) {
        m_backupPathEdit->setText(dir);
    }
}