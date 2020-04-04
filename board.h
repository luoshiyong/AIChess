#ifndef BOARD_H
#define BOARD_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QProgressBar>
#include "global.h"
#include <QPushButton>
class Board : public QMainWindow
{
    Q_OBJECT
public:
    /*
     * 最大最小值算法
    */
    void retool(mov top);
    int buju1[90];      //保存原始数据
    int weizhi1[32];
    QPushButton *regret;    //悔棋
    QPushButton *newgame;
    QVector<mov> record;
    QVector<mov> peo;       //人走记录
    QVector<mov> pc;        //电脑走记录
    explicit Board(QWidget *parent = 0);
    int size,flag,times,control;          //size为棋盘大小参数，flag为鼠标点击的标志位，control用来控制分出胜负后不再走棋
    QLabel *chess[32];                      //32个棋子
    QPixmap pic[34];
    QLabel *labelMousePos;     //走棋框
    QLabel *tips;                           //提示哪一方走棋以及是否被将军
    QProgressBar *progress;   //进度条
    void refresh(int *torefresh);
    bool hasclick;     //是否已经选中棋子
    int doubleclick;
    int haschess(int m);  //判断该处是否有棋子，有返回标识符，没有-1
    QLabel *red;
    QLabel *black;
    //走棋规则
    int gameover();
    bool canmove(int chessid,int captureid,int from,int to);
    int isturn;
    int level;    //考虑多少层嵌套==难度
    QVector<mov> getallstep(int op);  //0--表示电脑可以走的
                                    //1--表示人可以走的
    void fakemove(mov step);
    void unfakemove(mov step);
    int ctscore();          //计算局面分
    int getmaxscore(int level,int curminscore);
    int getminscore(int level,int curmaxscore);
signals:
    void humanfin();
    void pcfin();
protected:
    void mousePressEvent(QMouseEvent *e);//鼠标点击的响应函数
    void paintEvent(QPaintEvent *);   //绘制棋盘
public slots:
    mov getbestmove();
    void restohf();
    void restocf();
    void restohq();
    void restonew();
};

#endif // BOARD_H
