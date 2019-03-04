#ifndef MEMBERDATA_H
#define MEMBERDATA_H

#include <QDialog>
#include "member.h"
#include "node.h"
#include "mainwindow.h"
#include "library.h"
#include "saslibrary_global.h"
namespace Ui {
class memberData;
}

class SASLIBRARYSHARED_EXPORT memberData : public QDialog
{
    Q_OBJECT
    MAKE_TESTABLE(memberData);
public:
    explicit memberData(QWidget *parent = nullptr);
    ~memberData();

private slots:
    void on_memberList_itemClicked(QListWidgetItem *item);

    void on_refresh_released();

private:
    Ui::memberData *ui;
};

#endif // MEMBERDATA_H
