#ifndef ACCOUNTCARD_H
#define ACCOUNTCARD_H
#include <QWidget>
class AccountCard : public QWidget {
    Q_OBJECT
public:
    explicit AccountCard(QWidget* parent = nullptr);
};
#endif // ACCOUNTCARD_H