#include "drawmodel.h"

DrawModel::DrawModel(QWidget *parent) : QWidget(parent)
{
    width = 8;
    height = 8;
    scaleFactorX = 512/width;
    scaleFactorY = 512/height;
    QImage newPicture =  QImage(width, height, QImage::Format_ARGB32);
    newPicture.fill(qRgb(12,155,134));
    QPainter painter(&newPicture);
    painter.drawImage(QPoint(0,0), newPicture);
    picture = newPicture;
    drawGrid();
}


void DrawModel::paintEvent(QPaintEvent * paintEvent)
{
    QPainter painter(this);
    QRect rectangle = paintEvent->rect();
    painter.scale(scaleFactorX, scaleFactorY);
    painter.drawImage(rectangle, picture, rectangle);
}

void DrawModel::mouseMoveEvent(QMouseEvent* mouseEvent)
{
    QPoint point(mouseEvent->pos());
    int x = point.x()/scaleFactorX;
    int y = point.y()/scaleFactorY;
    drawALine(lastPoint, QPoint(x,y), Qt::red);

}

void DrawModel::mousePressEvent(QMouseEvent* mouseEvent)
{
    QPoint point(mouseEvent->pos());
    int x = point.x()/scaleFactorX;
    int y = point.y()/scaleFactorY;
    drawAPoint(QPoint(x,y), Qt::red);

}

void DrawModel::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
    QPoint point(mouseEvent->pos());
    lastPoint = point/32;
}

void DrawModel::drawAPoint(QPoint pos, QColor color)
{

    QPainter painter(&picture);
    painter.setPen(QPen(color, 1));
    painter.drawPoint(pos.x(), pos.y());
    lastPoint = pos;
    update();
}

void DrawModel::drawGrid()
{
    QColor* color = new QColor();
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            if((i + j) % 2 == 0)
            {
                color = new QColor(100, 100, 100, 255);
            }
            else
            {
                color = new QColor(150, 150, 150, 255);
            }
            drawAPoint(*(new QPoint(i, j)), *color);
        }
    }
}

void DrawModel::drawALine(QPoint lastPos, QPoint currentPos, QColor color)
{
    QPainter painter(&picture);
    painter.setPen(QPen(color, 1));
    painter.drawLine(lastPos, currentPos);
    lastPoint = currentPos;
    update();
}

void DrawModel::userGivenWidthAndHeight(int passedWidth, int passedHeight)
{
    std::cout << "HELLO! ITS ME!! I WAS WONDERING AFTER ALL THESE YEARS YOUD LIKE TO MEET?!" << std::endl;
    width = passedWidth;
    height = passedHeight;
    scaleFactorX = 512/width;
    scaleFactorY = 512/height;

    QImage newPicture =  QImage(width, height, QImage::Format_ARGB32);
    newPicture.fill(qRgb(255,255,255));
    QPainter painter(&newPicture);
    painter.drawImage(QPoint(0,0), newPicture);
    picture = newPicture;
    drawGrid();
}
/*
void DrawModel::resizeEvent(QResizeEvent *event)
{
    scaleFactorX = this->width/width;
    scaleFactorY = this->height/height;
}

*/

