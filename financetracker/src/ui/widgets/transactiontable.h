#ifndef TRANSACTIONTABLE_H
#define TRANSACTIONTABLE_H

#include <QTableWidget>

class TransactionTable : public QTableWidget {
    Q_OBJECT
public:
    explicit TransactionTable(QWidget* parent = nullptr);
};

#endif // TRANSACTIONTABLE_H