#include "choose.h"
#include <QDebug>
#include "global.h"
Choose::Choose()
{
    qDebug()<<"begin!!!!!!";
    this->resize(800,800);
    this->move(400,50);
    computer = new QPushButton(this);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/pic/bg.jpg").scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    this->setPalette(palette);
    computer->setStyleSheet("QPushButton{border-image:url(:/pic/pc.PNG);background-position: center;}");
    computer->setGeometry(250,350,300,60);
    connect(computer,SIGNAL(clicked()),this,SLOT(accept()));
    connect(computer,SIGNAL(clicked()),this,SLOT(selectcomputer()));


    qDebug()<<"end!";

}

void Choose::selectcomputer()
{
    mode = 0;
}

