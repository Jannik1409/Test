#ifndef TAGSDIALOG_H
#define TAGSDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "../../database/database.h"

class TagsDialog : public QDialog {
    Q_OBJECT
public:
    explicit TagsDialog(Database& db, QWidget* parent = nullptr);
    
private slots:
    void onAdd();
    void onDelete();
    
private:
    void loadTags();
    
    Database& m_db;
    std::vector<Tag> m_tags;
    
    QListWidget* m_list;
    QLineEdit* m_nameEdit;
    QPushButton* m_addButton;
    QPushButton* m_deleteButton;
    QPushButton* m_closeButton;
};

#endif // TAGSDIALOG_H
