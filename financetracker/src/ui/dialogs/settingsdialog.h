#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    
private slots:
    void onSave();
    void onThemeChanged(int index);
    void onBrowse();

private:
    void loadSettings();
    void saveSettings();
    
    QComboBox* m_themeCombo;
    QComboBox* m_currencyCombo;
    QComboBox* m_dateFormatCombo;
    QCheckBox* m_notificationsCheck;
    QLineEdit* m_backupPathEdit;
    QPushButton* m_browseBtn;
    QPushButton* m_saveButton;
    QPushButton* m_cancelButton;
};

#endif // SETTINGSDIALOG_H