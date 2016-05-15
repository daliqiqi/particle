#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QPainter>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    appTimerId=startTimer(50);//
    paintFlag=false;
    for(int i=0;i<50;i++)
    {
        starX[i]=rand()%this->width();
        starY[i]=rand()%this->height();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    curX=e->x();
    curY=e->y();
    makeFire();
}
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(0,0,0)));
    painter.drawRect(0,0,this->width(),this->height());
    QPen pen;
    pen.setBrush(QColor(255,255,255));
    pen.setColor(QColor(255,255,255));
    for(int i=0;i<50;i++)
    {
        pen.setWidth(rand()%2+1);
        painter.setPen(pen);
        painter.drawPoint(starX[i],starY[i]);
    }
    if(paintFlag)
    {
        drawView(painter);
        paintFlag=false;
    }
}
void MainWindow::drawView(QPainter &painter)
{
    int size=fire.size();
    QPen pen;
    for(int i=0;i<size;i++)
        for(int j=0;j<1000;j++)
        {
            if(fire[i].isShow[j])
            {
                pen.setWidth(rand()%2+1);
                pen.setColor(QColor(fire[i].r[j],fire[i].g[j],fire[i].b[j],rand()%55+200));
                painter.setPen(pen);
                painter.drawPoint(fire[i].x[j],fire[i].y[j]);
            }
        }
}
void MainWindow::makeFire()
{
    GRANULE *temp=new GRANULE;
    for(int i=0;i<1000;i++)
    {
        temp->curX=curX;
        temp->curY=curY;
        temp->x[i]=curX;
        temp->y[i]=curY;
        temp->isShow[i]=true;
        temp->r[i]=rand()%255;
        temp->g[i]=rand()%255;
        temp->b[i]=rand()%255;
        temp->count=1000;
        temp->acc=-10;
        temp->vx[i]=rand()%50-25;
        temp->vy[i]=rand()%50-25;
    }
    fire.append(*temp);
    delete temp;
    temp=NULL;

}
void MainWindow::timerEvent(QTimerEvent *event)//Ê±ÖÓÖÜÆÚ
{
    if(event->timerId()==appTimerId)
    {
        int size=fire.size();
        if(size<0)
            return;
        for(int i=0;i<size;i++)
        {
            for(int j=0;j<1000;j++)
            {
                if(fire[i].isShow[j])
                {
                    fire[i].x[j]+=fire[i].vx[j];
                    fire[i].y[j]+=fire[i].vy[j];
                    fire[i].vy[i]+=fire[i].acc;
                    QPainterPath path;
                    path.addEllipse(QPointF(fire[i].curX,fire[i].curY),300,300);
                    if(!path.contains(QPointF(fire[i].x[j],fire[i].y[j])))
                    //if (fire[i].x[j]>this->width()||fire[i].x[j]<0||fire[i].y[j]>this->height()||fire[i].y[j]<0)
                    {
                        fire[i].isShow[j]=false;
                        fire[i].count--;
                    }
                }
            }
        }
        for(int t=0;t<size;t++)
        {
            if(fire[t].count<100)
            {
                fire.remove(t);
                break;
            }
        }
        paintFlag=true;
        repaint();
    }

}
void MainWindow::changeEvent(QEvent *)
{
    for(int i=0;i<50;i++)
    {
        starX[i]=rand()%this->width();
        starY[i]=rand()%this->height();
    }
}
