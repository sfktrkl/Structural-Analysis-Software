#ifndef STRUCTUREDATA_H
#define STRUCTUREDATA_H

#include <QDialog>
#include "node.h"
#include "mainwindow.h"
#include "saslibrary_global.h"
namespace Ui {
class structureData;
}

class SASLIBRARYSHARED_EXPORT structureData : public QDialog
{
    Q_OBJECT
    MAKE_TESTABLE(structureData);
public:
    explicit structureData(QWidget *parent = nullptr);
    ~structureData();

private slots:
    void on_refresh_released();

private:
    Ui::structureData *ui;
};

#endif // STRUCTUREDATA_H
