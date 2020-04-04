#include "board.h"
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QDebug>
#include <cstring>
#include <QVector>
mov st;  //记录每一步走棋
Board::Board(QWidget *parent) : QMainWindow(parent)
{
    //int posx(int n,int size);
    //int posy(int n,int size);
    doubleclick = 0;
    isturn = 0;    //默认红棋先走
    size = 60;   //棋盘大小
    level = 4;
    this->setWindowTitle(tr("中国象棋"));
    this->setFixedSize(size*10+100,size*10+60);                 //固定窗口大小
    this->move(400,0);

    //保存原始数据
    memcpy(buju1,buju,sizeof(buju));
    memcpy(weizhi1,weizhi,sizeof(weizhi));
    //走棋框
    labelMousePos=new QLabel(this);
    labelMousePos->setText(tr(""));
    labelMousePos->setFixedWidth(100);
    //悔棋
    regret = new QPushButton(this);
    regret->setText("悔棋");
    regret->setGeometry(570,180,120,50);
    regret->show();
    //新游戏
    newgame = new QPushButton(this);
    newgame->setText("新的一局");
    newgame->setGeometry(570,100,120,50);
    newgame->show();
    //提示哪方走棋及将军
    tips = new QLabel(this);
    tips->setGeometry(600,370,120,120);
    tips->setText("红方走棋");
    //进度条
    progress=new QProgressBar(this);
    progress->setGeometry(580,550,100,20);
    progress->hide();
    //
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/pic/blue.png")));
    this->setPalette(palette);
    update();


    pic[0]=QPixmap(":/pic/blackgen.png");  //帅
    pic[1]=QPixmap(":/pic/blackadv.png");//士
    pic[2]=QPixmap(":/pic/blackadv.png"); //士
    pic[3]=QPixmap(":/pic/blackele.png");//象
    pic[4]=QPixmap(":/pic/blackele.png");//象
    pic[5]=QPixmap(":/pic/blackhorse.png");//马
    pic[6]=QPixmap(":/pic/blackhorse.png");//马
    pic[7]=QPixmap(":/pic/blackchar.png");//车
    pic[8]=QPixmap(":/pic/blackchar.png");//车
    pic[9]=QPixmap(":/pic/blackcan.png");//炮
    pic[10]=QPixmap(":/pic/blackcan.png");//炮
    pic[11]=QPixmap(":/pic/blacksod.png");//兵
    pic[12]=QPixmap(":/pic/blacksod.png");//兵
    pic[13]=QPixmap(":/pic/blacksod.png");//兵
    pic[14]=QPixmap(":/pic/blacksod.png");//兵
    pic[15]=QPixmap(":/pic/blacksod.png");//兵
    pic[16]=QPixmap(":/pic/redgen.png");
    pic[17]=QPixmap(":/pic/redadv.png");
    pic[18]=QPixmap(":/pic/redadv.png");
    pic[19]=QPixmap(":/pic/redele.png");
    pic[20]=QPixmap(":/pic/redele.png");
    pic[21]=QPixmap(":/pic/redhorse.png");
    pic[22]=QPixmap(":/pic/redhorse.png");
    pic[23]=QPixmap(":/pic/redchar.png");
    pic[24]=QPixmap(":/pic/redchar.png");
    pic[25]=QPixmap(":/pic/redcan.png");
    pic[26]=QPixmap(":/pic/redcan.png");
    pic[27]=QPixmap(":/pic/redsod.png");
    pic[28]=QPixmap(":/pic/redsod.png");
    pic[29]=QPixmap(":/pic/redsod.png");
    pic[30]=QPixmap(":/pic/redsod.png");
    pic[31]=QPixmap(":/pic/redsod.png");
    pic[32]=QPixmap(":/pic/redboundary.png");
    pic[33]=QPixmap(":/pic/blackboundary.png");
    //定义棋子
    int i=0;
    for(;i<=31;i++)
    {
        chess[i]=new QLabel(this);
        chess[i]->setPixmap(pic[i]);
        chess[i]->setGeometry(getx(weizhi[i])-30,gety(weizhi[i])-30,56,56);
    }


    red=new QLabel(this);
    red->setPixmap(pic[32]);
    red->hide();
    black=new QLabel(this);
    black->setPixmap(pic[33]);
    black->hide();
    init();
    /*for(int i =1;i<=90;i++)
    {
        qDebug()<<" "<<total[i];
    }*/
    connect(this,SIGNAL(humanfin()),this,SLOT(restohf()));
    connect(this,SIGNAL(pcfin()),this,SLOT(restocf()));
    connect(regret,SIGNAL(clicked(bool)),this,SLOT(restohq()));
    connect(newgame,SIGNAL(clicked(bool)),this,SLOT(restonew()));

}


void Board::paintEvent(QPaintEvent *)                                   //绘制棋盘格线
{
    //short posx(short n,short size);
   // short posy(short n,short size);

    QPainter p;
    p.begin(this);
    QPen background=p.pen();
    background.setWidth(2);
    p.setPen(background);
    p.drawLine(size-5,size-5,size*9+5,size-5);
    p.drawLine(size-5,size-5,size-5,size*10+5);
    p.drawLine(size-5,size*10+5,size*9+5,size*10+5);
    p.drawLine(size*9+5,size-5,size*9+5,size*10+5);
    background.setWidth(1);
    p.setPen(background);
    short n=1;
    for(;n<10;n++)
    {
        p.drawLine(size*n,size,size*n,size*5);
        p.drawLine(size*n,size*6,size*n,size*10);//Vertical
    }
    p.drawLine(size,size*5,size,size*6);
    p.drawLine(size*9,size*5,size*9,size*6);
    for(n=1;n<=10;n++)
        p.drawLine(size,size*n,size*9,size*n);//Horizon

    p.drawLine(size*4,size,size*6,size*3);              //斜线轨迹
    p.drawLine(size*6,size,size*4,size*3);
    p.drawLine(size*4,size*8,size*6,size*10);
    p.drawLine(size*6,size*8,size*4,size*10);
    p.end();
}
int Board::haschess(int m)
{
    return buju[m];
}
bool Board::canmove(int chessid, int captureid, int from, int to)
{
    /*
     * 1. 人工方操作16-31号棋子
       2.本函数实现走棋规则          */
    bool is = false;
    //qDebug()<<"from---to is"<<from<<" "<<to;
    switch(chessid)
    {
        case 0:
        case 16:         //帅
        {
             if((qAbs(from-to)==1&&findm(to,jiang,18))||(qAbs(from-to)==9&&findm(to,jiang,18)))
                is = true;
             break;
        }
        case 1:
        case 2:
        case 17:   //士
        case 18:
        {
            if(qAbs(from-to)==8&&findm(to,shi,10)||qAbs(from-to)==10&&findm(to,shi,10))
                is = true;
            break;
        }
        case 3:
        case 4:
        case 19:  //象
        case 20:
        {
            if(qAbs(from-to)==16&&findm(to,xiang,14)||qAbs(from-to)==20&&findm(to,xiang,14))
                is = true;
            break;
        }
        case 5:
        case 6:
        case 21:  //马
        case 22:
        {
            int x1 = from%9;
            int y1 = from/9;
            int x2 = to%9;
            int y2 = to/9;
            if(y2-y1==2&&qAbs(x2-x1)==1&&buju[(y1+1)*9+x1]==-1)
                is = true;
            if(y1-y2==2&&qAbs(x2-x1)==1&&buju[(y1-1)*9+x1]==-1)
                is = true;
            if(x2-x1==2&&qAbs(y2-y1)==1&&buju[y1*9+x1+1]==-1)
                is = true;
            if(x1-x2==2&&qAbs(y2-y1)==1&&buju[y1*9+x1-1]==-1)
                is = true;
            //if(is==false)
               // qDebug()<<"ma cannot move from "<<from<<" to " <<to;
            break;
        }
        case 7:
        case 8:
        case 23:   //车
        case 24:
        {

            if(from/9==to/9)   //同一行
            {
                int p = qMax(from,to);
                int q = qMin(from,to);
                int ct = 0;
                for(int i = q+1;i<p;i++)
                {
                    if(ct>=1)break;
                    if(buju[i]!=-1)
                        ct++;
                }
                if(ct==0)is = true;
            }
            if(from%9==to%9)
            {
                int p = qMax(from,to);
                int q = qMin(from,to);
                int ct = 0;
                for(int i = q+9;i<p;i+=9)
                {
                    if(ct>=1)break;
                    if(buju[i]!=-1)
                        ct++;
                }
                if(ct==0)is = true;
            }
            break;
        }
        case 9:
        case 10:
        case 25:   //炮
        case 26:
        {
            if(from/9==to/9)   //同一行
            {
                int p = qMax(from,to);
                int q = qMin(from,to);
                int ct = 0;
                for(int i = q+1;i<p;i++)
                {
                    if(ct>1)break;
                    if(buju[i]!=-1)
                        ct++;
                }
                if(ct==1&&captureid!=-1)is = true;
                if(ct==0&&captureid==-1)is = true;
            }
            else if(from%9==to%9)  //同一列
            {
                int p = qMax(from,to);
                int q = qMin(from,to);
                int ct = 0;
                for(int i = q+9;i<p;i+=9)
                {
                    if(ct>1)break;
                    if(buju[i]!=-1)
                        ct++;
                }
                if(ct==1&&captureid!=-1)is = true;
                if(ct==0&&captureid==-1)is = true;
            }
            break;
        }
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        {
            if(from<=44&&to-from==9&&chessid<16) //本土----只可以向前走
            {
                is = true;
           // qDebug()<<"in bentu!";
            }
            if(chessid<16&&from>44&&(qAbs(from-to)==1||(to-from)==9)) //境外----不可以后退
            {
                      is = true;
                  //    qDebug()<<"in guowai!";
            }
            if(from>44&&from-to==9&&chessid>15) //本土----只可以向前走
            {
                is = true;
               // qDebug()<<"in bentu!";
            }
            if(chessid>15&&from<=44&&(qAbs(from-to)==1||(from-to)==9)) //境外----不可以后退
            {
                          is = true;
                      //    qDebug()<<"in guowai!";
            }
            break;
        }


    }
    return is;
}
/*
 * 获取红方或者黑方当前所有可以走的步数
 * op=1->人
 * op=0->电脑
*/
QVector<mov> Board::getallstep(int op)
{
    QVector<mov> steps;
    int left,right;
    if(op==1)
    {
        left = 16;
        right = 32;
    }
    else
    {
        left = 0;
        right = 16;
    }
    for(int i = left;i<right;i++)   //黑方的所有棋子
    {
        if(weizhi[i]==-1){
            //qDebug()<<"chess "<<i<<"be eated!";
            continue;
        }
         //qDebug()<<"chess "<<i<<"is thinking!";
        for(int j = 0;j<90;j++)  //棋盘所有的位置
        {
             mov ct(i,buju[j],weizhi[i],j);
            if(buju[j]<right&&buju[j]>=left)
                continue;  //该位置位己方棋子
           //if(i==0) qDebug()<<"chess "<<i<<" from "<<weizhi[i]<<" to "<<j<<" is thinking!";
            //qDebug()<<"the  move info name--"<<ct.name<<" ";
            //qDebug()<<"the  move info capture--"<<ct.capture<<" ";
            //qDebug()<<"the  move info from--"<<ct.from<<" ";
            //qDebug()<<"the  move info to--"<<ct.to<<" ";
            if(canmove(i,buju[j],weizhi[i],j))
            {

                steps.push_back(ct);
            }
        }
    }
    //qDebug()<<"total passible step---"<<steps.count();
    return steps;
}
void Board::fakemove(mov step)
{
    if(step.capture!=-1)  //吃棋子
    {
        //改变布局和位置
          weizhi[step.capture] = -1;//吃对方棋子
          weizhi[step.name] = step.to; //移动己方棋子
          buju[step.from] = -1;   //己方原位置为空
          buju[step.to] = step.name;  //移动后
          //qDebug()<<"fakemove(!=-1) pao position is---"<<weizhi[9];
    }
    else   //移动
    {
        //改变布局和位置
             weizhi[step.name] = step.to;
             buju[step.from] = -1;
             buju[step.to] = step.name;
               // qDebug()<<"fakemove(=-1) pao position is---"<<weizhi[9];
             //qDebug()<<"fakemove(=-1)m=32 position is---"<<buju[31];
    }
}
void Board::unfakemove(mov step)
{
    if(step.capture!=-1)  //吃棋子
    {
        //改变布局和位置
          weizhi[step.capture] = step.to;//放回对方棋子
          weizhi[step.name] = step.from; //移回己方棋子
          buju[step.from] = step.name;   //
          buju[step.to] = step.capture;  //
          //qDebug()<<"unfakemove(!=-1) pao position is---"<<weizhi[9];
    }
    else   //移动
    {
        //改变布局和位置
             weizhi[step.name] = step.from;
             buju[step.from] = step.name;
             buju[step.to] = -1;
             //qDebug()<<"fakemove(=-1) pao position is---"<<weizhi[9];
             //qDebug()<<"unfakemove(=-1)m=32 position is---"<<buju[31];
    }
}
//评价局面分
int Board::ctscore()
{
    int redscore = 0,blackscore = 0;
    //CHE,MA,PAO,BING,SHI,JIANG,SHI,XIANG
    //黑棋分减去红棋分
    for(int i = 0;i<16;i++ )
    {
        if(weizhi[i]==-1)continue; //棋子i被吃了
        blackscore+=chescore[i];
    }
    for(int i = 16;i<32;i++ )
    {
        if(weizhi[i]==-1)continue; //棋子i被吃了
        redscore+=chescore[i];
    }
    return blackscore-redscore;
}
mov Board::getbestmove()
{
    /*
     * 1.获取所有可能走的步数
     * 2.试着走一下
     * 3.看看局面怎么样
     * 4.选择最好的走法
    */
    //qDebug()<<"begin to get best move!!!";
    QVector<mov> canstep;
    canstep = getallstep(0); //电脑走法
    qDebug()<<"allstep ==="<<canstep.count();
    int maxscore = -100000;
    mov ret;
    for(auto it = canstep.begin();it!=canstep.end();it++)
    {
        mov step = *it;
        //qDebug()<<"origin pao position is---"<<weizhi[9];
        //qDebug()<<"chess "<<step.name<<" from "<<step.from<<" to "<<step.to;
        fakemove(step);
        int score = getminscore(level-1,maxscore);
        unfakemove(step);
        qDebug()<<"THE SCORE IS--"<<score;
        if(score>maxscore)
        {
            maxscore = score;
            ret.equal(step);
        }
    }
    return ret;
}
int Board::getmaxscore(int level,int curminscore)
{
    if(level==0)
        return ctscore();
    QVector<mov> step;
    step = getallstep(0);   //获取人可以走的步数
    int maxscore = -100000;
    for(auto it = step.begin();it!=step.end();it++)
    {
        mov mystep = *it;
        //qDebug()<<"chess "<<step.name<<" from "<<step.from<<" to "<<step.to;
        fakemove(mystep);
        int score = getminscore(level-1,maxscore);
        unfakemove(mystep);
        if(score>=curminscore)
            return score;
        //qDebug()<<"THE SCORE IS--"<<score;
        if(score>maxscore)
        {
            maxscore = score;
        }
    }
    return maxscore;
}
int Board::getminscore(int level,int curmaxscore)
{
    if(level==0)
        return ctscore();
    QVector<mov> step;
    step = getallstep(1);   //获取人可以走的步数
    int minscore = 100000;
    for(auto it = step.begin();it!=step.end();it++)
    {
        mov mystep = *it;
        //qDebug()<<"chess "<<step.name<<" from "<<step.from<<" to "<<step.to;
        fakemove(mystep);
        int score = getmaxscore(level-1,minscore);
        unfakemove(mystep);
        if(curmaxscore>=score)
            return score;
        //qDebug()<<"THE SCORE IS--"<<score;
        if(score<minscore)
            minscore = score;
    }
    return minscore;
}
void Board::restohf()
{
    qDebug()<<"human finished!!!!";
    //判断游戏是否结束
    if(gameover()==1)
    {
        tips->setText(tr("人类获胜"));
        update();
        qDebug()<<"人类获胜";
        return;
    }
    isturn = 1;
    tips->setText(tr("黑方走棋"));
    update();
    mov tmove = getbestmove();
    record.push_back(tmove);
    pc.push_back(tmove);
    qDebug()<<"the best move info name--"<<tmove.name<<" ";
    qDebug()<<"the best move info capture--"<<tmove.capture<<" ";
    qDebug()<<"the best move info from--"<<tmove.from<<" ";
    qDebug()<<"the best move info to--"<<tmove.to<<" ";
    qDebug()<<"begin to play !!!";
    if(tmove.capture!= -1)
    {
        chess[tmove.name]->setGeometry(getx(tmove.to)-30,gety(tmove.to)-30,56,56);
        chess[tmove.capture]->hide();
      //改变布局和位置
        weizhi[tmove.capture] = -1;
        weizhi[tmove.name] = tmove.to;
        buju[tmove.from] = -1;
        buju[tmove.to] = tmove.name;
        //qDebug()<<"computer done(!=-1) pao position is---"<<weizhi[9];
        emit pcfin();
      //
    }
    else
    {
        chess[tmove.name]->setGeometry(getx(tmove.to)-30,gety(tmove.to)-30,56,56);
   //改变布局和位置
        weizhi[tmove.name] = tmove.to;
        buju[tmove.from] = -1;
        buju[tmove.to] = tmove.name;
        emit pcfin();
        //qDebug()<<"computer done(=-1) pao position is---"<<weizhi[9];
    }

}
void Board::restocf()
{
    isturn = 0;
    //判定将军

    tips->setText(tr("红方走棋"));

}
void Board::mousePressEvent(QMouseEvent *e) //鼠标点击的响应函数
{
    /*用于实现人走棋*/
   int m = num(e->x(),e->y(),size);
   qDebug()<<"m is---"<<m;
   qDebug()<<"buju[m] is---"<<buju[m];
   qDebug()<<"xiangsu x = "<<e->x();
   qDebug()<<"xiangsu y = "<<e->y();
   qDebug()<<"xiangsuget x = "<<getx(m);
   qDebug()<<"xiangsuget y = "<<gety(m);
   if(isturn==0)
   {
        int current = haschess(m);  //返回棋子id,查看点中的是什么棋子或者没有棋子
        qDebug()<<"current click is---"<<current;
        if(current!= -1)  //点中棋子
         {
            //先判断是吃还是选中，根据第几次选中
            if(doubleclick == 1)
             {
                 if(current>15) //重新选择走棋的棋子（第二次点击是另一个本方棋子）
                 {
                     red->setGeometry(getx(m)-30,gety(m)-30,56,56);
                     red->show();
                     st.name = current;
                     st.from = m;
                     return;
                 }
                //移动是否合法
                 //吃m位置的棋子
                 st.capture =current;
                 st.to = m;
                 if(canmove(st.name,st.capture,st.from,st.to)) //是否符合规则
                 {
                    doubleclick++;

                    qDebug()<<"two eat !!";
                    red->hide();
                    chess[st.name]->setGeometry(getx(m)-30,gety(m)-30,56,56);
                    chess[st.capture]->hide();
                    //改变布局和位置
                    weizhi[st.capture] = -1;
                    weizhi[st.name] = st.to;
                    buju[st.from] = -1;
                    buju[st.to] = st.name;
          //

                    emit humanfin();    //人走完信号
                    record.push_back(st);
                    peo.push_back(st);
                    st.clear();
                }
                else
                    qDebug()<<"cannot move becase rules!";
            }
            else
            {
                if(current<16)return ;  //人工仅能操作红棋
                doubleclick++;
                qDebug()<<"the first click---!";
                st.clear();   //清除st中的数据
                red->setGeometry(getx(m)-30,gety(m)-30,56,56);
                red->show();
                st.name = current;
                st.from = m;
            }
        }
        else  //未点中棋子
        {
            //可能走棋到空位，也可能是无效点击
            if(doubleclick == 1)  //走棋到空位
            {
                st.capture = -1;
                st.to = m;
                if(canmove(st.name,st.capture,st.from,st.to))
                {
                    doubleclick++;
                    red->hide();
                    qDebug()<<"two kongwei !!--"<<st.name;
                    chess[st.name]->setGeometry(getx(m)-30,gety(m)-30,56,56);
                    //改变布局和位置
                    weizhi[st.name] = st.to;
                    buju[st.from] = -1;
                    buju[st.to] = st.name;
           //
                    qDebug()<<"pao position is---"<<weizhi[st.name];
                    qDebug()<<"m=32 position is---"<<buju[32];
                    emit humanfin();
                    record.push_back(st);
                    peo.push_back(st);
                    st.clear();//走棋完成清除数据
                }
                else
                    qDebug()<<"cannot move becase rules!";
       }
       else
       {
            qDebug()<<"useless click!- buju[m]--"<<buju[m];

       }
   }


   qDebug()<<"m = "<<m;
   if(doubleclick == 2)   //两次合法点击，完成走棋
   {
       doubleclick = 0;
   }
}
}
void Board::refresh(int *torefresh)               //刷新界面的函数
{
    int i=0;
    for(;i<=31;i++)
    {
        if(torefresh[i+16]!=0)
            chess[i]->setGeometry(posx(torefresh[i+16],size),posy(torefresh[i+16],size),56,56);
        else
            chess[i]->hide();                            //该棋子被吃掉
    }


}
void Board::retool(mov top)
{
    if(top.capture!= -1)    //这一步棋吃了棋子
    {
        chess[top.name]->setGeometry(getx(top.from)-30,gety(top.from)-30,56,56);
        chess[top.capture]->show();
      //改变布局和位置
        weizhi[top.capture] = top.to;
        weizhi[top.name] = top.from;
        buju[top.from] = top.name;
        buju[top.to] = top.capture;
    }
    else                    //这一步棋没吃棋子
    {
        chess[top.name]->setGeometry(getx(top.from)-30,gety(top.from)-30,56,56);
   //改变布局和位置
        weizhi[top.name] = top.from;
        buju[top.from] = top.name;
        buju[top.to] = -1;

    }
}
void Board::restohq()
{
    /*悔棋*/
    //qDebug()<<"==============begin huiqi !===========";
    //qDebug()<<"pc  "<<pc.size();
    //qDebug()<<"human  "<<peo.size();
    //qDebug()<<"record  "<<record.size();
    int ss = record.size();
    if(ss<2||ss%2==1)return ;
    for(int i = 0;i<2;i++)
    {
        if(record.size()<1)continue;
        mov top = record.back();
        retool(top);
       // qDebug()<<"========"<<i<<"================";
        //qDebug()<<"top.nmame == "<<top.name;
        if(top.name<16)
        {
            pc.pop_back();
        }
        else
        {
            peo.pop_back();
        }
        record.pop_back();
    }

}
/*
 * function: judge who will win
 * return :
 * 1->human win
 * -1-> pc win
 * 0->continue;
*/
int Board::gameover()
{
    if(weizhi[0]==-1)
        return 1;
    else if(weizhi[16]==-1)
        return -1;
    else
        return 0;
}
/*
 * function:begin new game
*/
void Board::restonew()
{
    memcpy(buju,buju1,sizeof(buju1));
    memcpy(weizhi,weizhi1,sizeof(weizhi1));
    //重置所有参数
    for(int i = 0;i<=31;i++)
    {
        //qDebug()<<weizhi[i]<<" ";
        chess[i]->setPixmap(pic[i]);
        chess[i]->setGeometry(getx(weizhi[i])-30,gety(weizhi[i])-30,56,56);
        chess[i]->show();
    }
    update();
}
