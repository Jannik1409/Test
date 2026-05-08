#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>

class FilterDialog : public QDialog {
    Q_OBJECT
public:
    explicit FilterDialog(QWidget* parent = nullptr);
    QString getStartDate() const;
    QString getEndDate() const;
    QString getCategory() const;
    QString getType() const;
    QString getMinAmount() const;
    QString getMaxAmount() const;

private slots:
    void onApply();
    void onReset();

private:
    QDateEdit* m_startDateEdit;
    QDateEdit* m_endDateEdit;
    QComboBox* m_categoryCombo;
    QComboBox* m_typeCombo;
    QLineEdit* m_minAmountEdit;
    QLineEdit* m_maxAmountEdit;
};

#endif // FILTERDIALOG_H