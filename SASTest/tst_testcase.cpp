#include "mainwindow.h"
#include "library.h"
#include "member.h"
#include "node.h"
#include "memberdata.h"
#include "forcedata.h"
#include "structuredata.h"
#include "glwidget.h"
#include "saslibrary.h"
#include "saslibrary_global.h"

#include <QApplication>
#include <QtTest>
#include <QCoreApplication>
#include <QGLWidget>
#include <QtMath>
#include <QTimer>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/gl.h>
#include <QMainWindow>
#include <QListWidgetItem>
#include <QListWidget>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QtXml/QtXml>
#include <QtCore>
#include <QStandardPaths>
#include <QDialog>
#include <QString>
#include <QGLWidget>
#include <QTimer>

#include <vector>
#include <string>
#include <sstream>

#include "math.h"

class TestCase : public QObject
{
    Q_OBJECT
    friend class Member;
    friend class MainWindow;
public:
    TestCase();
    ~TestCase();

private slots:
    void initialValueTest();
    void transposeTest();
    void gaussJordanTest();
    void nodeTest();
    void memberTest();
    void firstExampleTest();
    void complexExampleTest();
    void xmlOpenTest();
    void xmlOpenTest2();
private:

};

TestCase::TestCase()
{
    QVERIFY(!MainWindow::solved);
}

TestCase::~TestCase()
{

}
void TestCase::initialValueTest()
{
    QCOMPARE(Node::numOfNodes,Member::numOfMembers);
}

void TestCase::transposeTest()
{
    std::vector<std::vector<double>> given{{1,1,1,1,1,1},{2,2,2,2,2,2},{3,3,3,3,3,3},{4,4,4,4,4,4},{5,5,5,5,5,5},{6,6,6,6,6,6}};
    std::vector<std::vector<double>> calculated = transpose(given);
    std::vector<std::vector<double>> verified{{1,2,3,4,5,6},{1,2,3,4,5,6},{1,2,3,4,5,6},{1,2,3,4,5,6},{1,2,3,4,5,6},{1,2,3,4,5,6}};
    QCOMPARE(calculated,verified);
}
void TestCase::gaussJordanTest(){
    std::vector<std::vector<double>> given{{1,2,4},{4,5,2},{1,2,3}};
    std::vector<std::vector<double>> given2{{1},{6},{3}};
    std::vector<std::vector<double>> calculated = gaussJordan(given,given2);
    QCOMPARE(calculated[2][0],double(-2));
}

void TestCase::nodeTest(){
    MainWindow::clearAll();
    MainWindow::nodes.push_back(Node(true,true,false,0,0,1000,0,0,100,0,0));
    MainWindow::nodes.push_back(Node(true,true,false,0,0,1000,0,0,100,0,0));
    QCOMPARE(Node::numOfNodes,static_cast<unsigned int>(2));
    QCOMPARE(Node::numOfDofs-1,static_cast<unsigned int>(2));
    QCOMPARE(MainWindow::nodes[0].GetStiffness()[2],1000);
    QCOMPARE(MainWindow::nodes[0].GetNodeForces()[0],100);
}

void TestCase::memberTest(){
    MainWindow::clearAll();
    MainWindow::nodes.push_back(Node(true,true,false,0,0,0,0,0,0,0,0));
    MainWindow::nodes.push_back(Node(true,true,false,0,0,1000,13,0,0,0,0));
    MainWindow::members.push_back(Member(0,1,20000,1,12,13,1,0));
    QCOMPARE(Member::numOfMembers,static_cast<unsigned int>(1));
    QCOMPARE(int(MainWindow::members[0].GetLocalMatrix()[0][0]),int(18461));
    QCOMPARE(int(MainWindow::members[0].GetRotationMatrix()[3][3]),int(1));
    QCOMPARE(int(MainWindow::members[0].GetGlobalMatrix()[5][5]),int(6153));
}

void TestCase::firstExampleTest(){
    MainWindow::clearAll();
    MainWindow::nodes.push_back(Node(true,true,false,0,0,0,0,0,0,0,0));
    MainWindow::nodes.push_back(Node(true,true,false,0,0,1000,13,0,0,0,0));
    MainWindow::members.push_back(Member(0,1,20000,1,12,13,1,0));
    MainWindow::members[0].SetLoads(-10,-10);
    QCOMPARE(int(MainWindow::members[0].GetLocalFEMMatrix()[1][0]),int(65));
    QCOMPARE(int(MainWindow::members[0].GetGlobalFEMMatrix()[4][0]),int(65));
}
void TestCase::complexExampleTest(){
    MainWindow::clearAll();
    MainWindow::nodes.push_back(Node(true,true,true,0,0,0,0,0,0,0,0));
    MainWindow::nodes.push_back(Node(false,false,false,0,100,0,5,0,0,0,0));
    MainWindow::nodes.push_back(Node(false,false,false,0,0,100,5,2,0,-100,0));
    MainWindow::nodes.push_back(Node(true,false,false,0,0,0,8,3,0,0,0));
    MainWindow::members.push_back(Member(0,1,20000,1,12,5,1,0));
    MainWindow::members[0].SetLoads(-10,-10);
    MainWindow::members.push_back(Member(1,2,20000,1,12,2,0,1));
    MainWindow::members.push_back(Member(2,3,20000,1,12,sqrt(10),3/sqrt(10),1/sqrt(10)));
    MainWindow::solveStructure();
    QCOMPARE(int(MainWindow::members[2].GetLocalMatrix()[2][1]),int(11999));
    QCOMPARE(int(MainWindow::members[2].GetGlobalMatrix()[1][1]),int(14419));
    QCOMPARE(int(MainWindow::members[0].GetLocalFEMMatrix()[1][0]),int(25));
    QCOMPARE(int(MainWindow::members[0].GetGlobalFEMMatrix()[1][0]),int(25));
    QCOMPARE(MainWindow::FGlobalMatrix[1][0],-25);
    bool comparison = MainWindow::Deflections[0][0] < -0.0015 && MainWindow::Deflections[0][0] > -0.0016;
    QVERIFY(comparison);

}
void TestCase::xmlOpenTest(){
    QString filename = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/test1.xml";
    MainWindow::OpenXML(filename);
    QCOMPARE(int(MainWindow::members[0].GetLocalFEMMatrix()[1][0]),int(845));
    QCOMPARE(int(MainWindow::members[0].GetGlobalFEMMatrix()[4][0]),int(845));
    MainWindow::solveStructure();
    QCOMPARE(int(MainWindow::FGlobalMatrix[0][0]),int(1830));
    bool comparison = MainWindow::Deflections[0][0] > 0.255 && MainWindow::Deflections[0][0] < 0.256;
    QVERIFY(comparison);
}

void TestCase::xmlOpenTest2(){
    QString filename = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/test2.xml";
    MainWindow::OpenXML(filename);
    MainWindow::solveStructure();
    QCOMPARE(int(MainWindow::members[2].GetLocalMatrix()[2][1]),int(11999));
    QCOMPARE(int(MainWindow::members[2].GetGlobalMatrix()[1][1]),int(14419));
    QCOMPARE(int(MainWindow::members[0].GetLocalFEMMatrix()[1][0]),int(25));
    QCOMPARE(int(MainWindow::members[0].GetGlobalFEMMatrix()[1][0]),int(25));
    QCOMPARE(MainWindow::FGlobalMatrix[1][0],-25);
    bool comparison = MainWindow::Deflections[0][0] < -0.0015 && MainWindow::Deflections[0][0] > -0.0016;
    QVERIFY(comparison);
}

QTEST_MAIN(TestCase)

#include "tst_testcase.moc"
