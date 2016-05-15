#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTime>
namespace Ui {
class MainWindow;
}
struct GRANULE
{
    int curX;
    int curY;
    int x[1000];//水平位置
    int y[1000];//竖直位置
    int vx[1000];//水平速度
    int vy[1000];//竖直速度
    bool isShow[1000];//是否显示
    //颜色
    int r[1000];
    int g[1000];
    int b[1000];
    int acc;//加速度
    int count;//总点数 1000

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    // 鼠标单击事件
   void mousePressEvent(QMouseEvent *event);
   void paintEvent(QPaintEvent *);
   void timerEvent(QTimerEvent *event);//时钟周期
   void changeEvent(QEvent *);
   void drawView(QPainter &painter);
   void makeFire();

private:
    Ui::MainWindow *ui;
public:
    QVector<GRANULE> fire;
    int curX;
    int curY;
    int appTimerId;
    bool paintFlag;
    int starX[50];
    int starY[50];
};

#endif // MAINWINDOW_H
