#ifndef SUPPORTREACTIONS_H
#define SUPPORTREACTIONS_H

#include <QDialog>
#include "mainwindow.h"
#include "library.h"

namespace Ui {
class supportReactions;
}

class supportReactions : public QDialog
{
    Q_OBJECT

public:
    explicit supportReactions(QWidget *parent = nullptr);
    ~supportReactions();

private slots:
    void on_refresh_released();

    void on_nodeList_itemClicked(QListWidgetItem *item);

private:
    Ui::supportReactions *ui;
};

#endif // SUPPORTREACTIONS_H
