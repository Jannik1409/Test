#include "transactiontable.h"

TransactionTable::TransactionTable(QWidget* parent)
    : QTableWidget(parent) {
    setColumnCount(6);
    setHorizontalHeaderLabels({"Date", "Description", "Category", "Type", "Amount", "Tags"});
}