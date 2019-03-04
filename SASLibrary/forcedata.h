#ifndef FORCEDATA_H
#define FORCEDATA_H

#include <QDialog>
#include "member.h"
#include "node.h"
#include "mainwindow.h"
#include "library.h"
#include "saslibrary_global.h"

namespace Ui {
class forceData;
}

class SASLIBRARYSHARED_EXPORT forceData : public QDialog
{
    Q_OBJECT
    MAKE_TESTABLE(forceData);
public:
    explicit forceData(QWidget *parent = nullptr);
    ~forceData();

private slots:
    void on_memberList_itemClicked(QListWidgetItem *item);

    void on_refresh_released();

    void on_nodeList_itemClicked(QListWidgetItem *item);

    void on_refresh_2_released();

private:
    Ui::forceData *ui;
};

#endif // FORCEDATA_H
