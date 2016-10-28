#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //central = new QWidget(this->centralWidget());
    artist = new DrawModel;
    size.show();
    size.raise();
    size.activateWindow();

    //mainSpace = new QGridLayout(ui->workspace);
    //ui->workspace->layout()->addWidget(artist);  //works because widget is also a grid layout
    spriteWidth = 16;
    spriteHeight = 16;

    //mainSpace->setColumnStretch(0,1);
    //mainSpace->setColumnStretch(3,1);
    //mainSpace->addWidget(artist,0,0,1,4);
    //setLayout(mainSpace);

    //connection between popup window and mainwindow
    QObject::connect(&size, &SizeSelector::setWidthAndHeight, this, &MainWindow::acceptWidthAndHeight);
    QObject::connect(this, &MainWindow::passWidthAndHeight, ui->workspace, &DrawModel::userGivenWidthAndHeight);
    QObject::connect(this, &MainWindow::setToolType, ui->workspace, &DrawModel::changeTools);
    QObject::connect(this, &MainWindow::setPenSize, ui->workspace, &DrawModel::changePenSize);
    //QObject::connect(this, &MainWindow::setPenColor, artist, &DrawModel::changePenColor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::acceptWidthAndHeight(int width, int height)
{
    spriteWidth = width;
    spriteHeight = height;
    this->show();
    emit passWidthAndHeight(spriteWidth, spriteHeight);
}

void MainWindow::on_penSizeSlider_valueChanged(int value)
{
    ui->penSizeLabel->setText(QString::number(value));
    emit setPenSize(value);
}

void MainWindow::on_penButton_clicked()
{
    emit setToolType("Pen");
}

void MainWindow::on_eraserButton_clicked()
{
    emit setToolType("Eraser");
}

void MainWindow::on_paintButton_clicked()
{
    emit setToolType("FillBucket");
}

void MainWindow::on_lineButton_clicked()
{
    emit setToolType("Line");
}

void MainWindow::on_ellipseButton_clicked()
{
    emit setToolType("Ellipse");
}

void MainWindow::on_rectangleButton_clicked()
{
    emit setToolType("Rectangle");
}
