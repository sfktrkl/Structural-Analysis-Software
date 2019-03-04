#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "member.h"
#include "node.h"

#include <QMainWindow>
#include <QListWidgetItem>
#include <QListWidget>
#include <QMessageBox>
#include "saslibrary_global.h"

namespace Ui {
class MainWindow;
}

class SASLIBRARYSHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT
    MAKE_TESTABLE(MainWindow);
    friend class TestCase;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static bool solved;
    static std::vector<Node> nodes;
    static std::vector<Member> members;
    static std::vector<std::vector<double>> KGlobalMatrix;
    static std::vector<std::vector<double>> FGlobalMatrix;
    static std::vector<std::vector<double>> Deflections;
    static void solveStructure();
    static void WriteXML(QString fileName);
    static void OpenXML(QString fileName);
    void drawNodes(bool bXFixity,bool bYFixity,bool bZFixity,int iXStiffness,int iYStiffness,int iZStiffness,double dbXCoordinate,double dbYCoordinate);
    static void clearAll();
    void createGrid(int xGrid,int yGrid);

private slots:
    void on_createNode_released();

    void on_createMember_released();

    void on_mWidth_textEdited();

    void on_mHeight_textEdited();

    void on_mNode1_textEdited();

    void on_mNode2_textEdited();

    void on_nodeList_itemClicked(QListWidgetItem *item);

    void on_changeNode_released();

    void on_memberList_itemClicked(QListWidgetItem *item);

    void on_changeMember_released();

    void on_unitWeight_textEdited();

    void on_SelfWeight_released();

    void on_addLoad_released();

    void on_actionMembers_triggered();

    void on_solve_released();

    void on_actionStructure_triggered();

    void on_actionForces_triggered();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    void on_createGrid_released();

    void on_xFixity_stateChanged(int arg1);

    void on_yFixity_stateChanged(int arg1);

    void on_zFixity_stateChanged(int arg1);



private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
