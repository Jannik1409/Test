#include "recurringdialog.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QDate>

RecurringDialog::RecurringDialog(Database& db, QWidget* parent)
    : QDialog(parent), m_db(db) {
    setWindowTitle("Recurring Transactions");
    resize(600, 450);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    m_list = new QListWidget(this);
    mainLayout->addWidget(m_list);
    
    QGridLayout* formLayout = new QGridLayout();
    
    formLayout->addWidget(new QLabel("Description:"), 0, 0);
    m_descriptionEdit = new QLineEdit(this);
    formLayout->addWidget(m_descriptionEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Amount:"), 1, 0);
    m_amountEdit = new QLineEdit(this);
    m_amountEdit->setPlaceholderText("0.00");
    formLayout->addWidget(m_amountEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Category:"), 2, 0);
    m_categoryCombo = new QComboBox(this);
    formLayout->addWidget(m_categoryCombo, 2, 1);
    
    formLayout->addWidget(new QLabel("Frequency:"), 3, 0);
    m_frequencyCombo = new QComboBox(this);
    m_frequencyCombo->addItem("Daily", "Daily");
    m_frequencyCombo->addItem("Weekly", "Weekly");
    m_frequencyCombo->addItem("Bi-weekly", "Biweekly");
    m_frequencyCombo->addItem("Monthly", "Monthly");
    m_frequencyCombo->addItem("Yearly", "Yearly");
    formLayout->addWidget(m_frequencyCombo, 3, 1);
    
    formLayout->addWidget(new QLabel("Start Date:"), 4, 0);
    m_startDateEdit = new QDateEdit(this);
    m_startDateEdit->setCalendarPopup(true);
    m_startDateEdit->setDate(QDate::currentDate());
    formLayout->addWidget(m_startDateEdit, 4, 1);
    
    formLayout->addWidget(new QLabel("End Date:"), 5, 0);
    m_endDateEdit = new QDateEdit(this);
    m_endDateEdit->setCalendarPopup(true);
    m_endDateEdit->setDate(QDate::currentDate().addYears(1));
    formLayout->addWidget(m_endDateEdit, 5, 1);
    
    m_activeCheck = new QCheckBox("Active", this);
    m_activeCheck->setChecked(true);
    formLayout->addWidget(m_activeCheck, 6, 1);
    
    mainLayout->addLayout(formLayout);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_addButton = new QPushButton("Add", this);
    m_editButton = new QPushButton("Edit", this);
    m_deleteButton = new QPushButton("Delete", this);
    m_closeButton = new QPushButton("Close", this);
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_editButton);
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_closeButton);
    mainLayout->addLayout(buttonLayout);
    
    loadCategories();
    loadRecurrings();
    
    connect(m_addButton, &QPushButton::clicked, this, &RecurringDialog::onAdd);
    connect(m_editButton, &QPushButton::clicked, this, &RecurringDialog::onEdit);
    connect(m_deleteButton, &QPushButton::clicked, this, &RecurringDialog::onDelete);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_list, &QListWidget::itemSelectionChanged, this, &RecurringDialog::onSelectionChanged);
}

void RecurringDialog::loadCategories() {
    m_categories = m_db.getAllCategories();
    m_categoryCombo->clear();
    for (const auto& cat : m_categories) {
        m_categoryCombo->addItem(QString::fromStdString(cat.getName()), cat.getId());
    }
}

void RecurringDialog::loadRecurrings() {
    m_recurrings = m_db.getAllRecurrings();
    m_list->clear();
    for (const auto& rec : m_recurrings) {
        QString text = QString::fromStdString(rec.getDescription()) + " - " +
                      QString::number(rec.getAmount(), 'f', 2) + " (" +
                      QString::fromStdString(rec.getFrequencyString()) + ")";
        QListWidgetItem* item = new QListWidgetItem(text, m_list);
        item->setData(Qt::UserRole, rec.getId());
    }
    updateButtons();
}

void RecurringDialog::updateButtons() {
    bool hasSelection = m_list->currentRow() >= 0;
    m_editButton->setEnabled(hasSelection);
    m_deleteButton->setEnabled(hasSelection);
}

void RecurringDialog::onAdd() {
    QString desc = m_descriptionEdit->text();
    QString amountStr = m_amountEdit->text();
    
    if (desc.isEmpty() || amountStr.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill in description and amount.");
        return;
    }
    
    double amount = amountStr.toDouble();
    if (amount <= 0) {
        QMessageBox::warning(this, "Error", "Amount must be greater than 0.");
        return;
    }
    
    Recurring rec;
    rec.setDescription(desc.toStdString());
    rec.setAmount(amount);
    rec.setCategoryId(m_categoryCombo->currentData().toInt());
    rec.setFrequency(Recurring::frequencyFromString(m_frequencyCombo->currentData().toString().toStdString()));
    rec.setStartDate(m_startDateEdit->date().toString("yyyy-MM-dd").toStdString());
    rec.setEndDate(m_endDateEdit->date().toString("yyyy-MM-dd").toStdString());
    
    int id = m_db.createRecurring(rec);
    if (id > 0) {
        loadRecurrings();
        m_descriptionEdit->clear();
        m_amountEdit->clear();
    } else {
        QMessageBox::critical(this, "Error", "Failed to create recurring transaction.");
    }
}

void RecurringDialog::onEdit() {
    int row = m_list->currentRow();
    if (row < 0) return;
    
    int id = m_list->currentItem()->data(Qt::UserRole).toInt();
    
    for (auto& rec : m_recurrings) {
        if (rec.getId() == id) {
            rec.setDescription(m_descriptionEdit->text().toStdString());
            rec.setAmount(m_amountEdit->text().toDouble());
            rec.setCategoryId(m_categoryCombo->currentData().toInt());
            rec.setFrequency(Recurring::frequencyFromString(m_frequencyCombo->currentData().toString().toStdString()));
            rec.setStartDate(m_startDateEdit->date().toString("yyyy-MM-dd").toStdString());
            rec.setEndDate(m_endDateEdit->date().toString("yyyy-MM-dd").toStdString());
            m_db.updateRecurring(rec);
            loadRecurrings();
            break;
        }
    }
}

void RecurringDialog::onDelete() {
    int row = m_list->currentRow();
    if (row < 0) return;
    
    if (QMessageBox::question(this, "Confirm", "Delete this recurring transaction?") == QMessageBox::Yes) {
        int id = m_list->currentItem()->data(Qt::UserRole).toInt();
        m_db.deleteRecurring(id);
        loadRecurrings();
    }
}

void RecurringDialog::onToggleActive() {
    int row = m_list->currentRow();
    if (row < 0) return;
    
    int id = m_list->currentItem()->data(Qt::UserRole).toInt();
    
    for (auto& rec : m_recurrings) {
        if (rec.getId() == id) {
            QString newEndDate = QDate::currentDate().toString("yyyy-MM-dd");
            rec.setEndDate(newEndDate.toStdString());
            m_db.updateRecurring(rec);
            loadRecurrings();
            break;
        }
    }
}

void RecurringDialog::onSelectionChanged() {
    int row = m_list->currentRow();
    if (row < 0) {
        updateButtons();
        return;
    }
    
    int id = m_list->currentItem()->data(Qt::UserRole).toInt();
    
    for (const auto& rec : m_recurrings) {
        if (rec.getId() == id) {
            m_descriptionEdit->setText(QString::fromStdString(rec.getDescription()));
            m_amountEdit->setText(QString::number(rec.getAmount(), 'f', 2));
            
            for (int i = 0; i < m_categoryCombo->count(); ++i) {
                if (m_categoryCombo->itemData(i).toInt() == rec.getCategoryId()) {
                    m_categoryCombo->setCurrentIndex(i);
                    break;
                }
            }
            
            QString freqStr = QString::fromStdString(rec.getFrequencyString());
            for (int i = 0; i < m_frequencyCombo->count(); ++i) {
                if (m_frequencyCombo->itemData(i).toString() == freqStr) {
                    m_frequencyCombo->setCurrentIndex(i);
                    break;
                }
            }
            
            m_startDateEdit->setDate(QDate::fromString(QString::fromStdString(rec.getStartDate()), "yyyy-MM-dd"));
            m_endDateEdit->setDate(QDate::fromString(QString::fromStdString(rec.getEndDate()), "yyyy-MM-dd"));
            m_activeCheck->setChecked(rec.isActive());
            break;
        }
    }
    
    updateButtons();
}