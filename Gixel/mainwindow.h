#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "setspritesize.h"
#include "spritedatasheet.h"
#include <QPainter>
#include "tool.h"
#include <QTimer>
#include "rendercanvas.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //QTimer updateTimer;
    Tool* currentTool; //Current working tool in the editor
    Pen pen;//Child of tool
    RenderCanvas *canvas;

public slots:
    void cancelSetSize();
    void setSpriteHeightAndWidth(int,int);
    //void placePoint(QPointF);

private slots:
    void on_PenToolButton_clicked();
    void workspaceClickCheck();

private:
    SetSpriteSize popupSize;
    SpriteDataSheet spriteData;
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
