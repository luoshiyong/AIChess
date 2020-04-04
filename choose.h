#ifndef CHOOSE_H
#define CHOOSE_H
#include <QDialog>
#include <QPushButton>
class Choose : public QDialog
{
public:
    Choose();

public slots:
    void selectcomputer();
private:
    QPushButton *computer;
};

#endif // CHOOSE_H
