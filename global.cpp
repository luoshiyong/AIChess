#include "global.h"
int mode = -1;
int piece[48]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                 0xc7,0xc6,0xc8,0xc5,0xc9,0xc4,0xca,0xc3,0xcb,0xa4,0xaa,0x93,0x95,0x97,0x99,0x9b,
                 0x37,0x36,0x38,0x35,0x39,0x34,0x3a,0x33,0x3b,0x54,0x5a,0x63,0x65,0x67,0x69,0x6b
};
int buju[90] = {            //棋盘某个点是哪个棋子
    7,5,3,1,0,2,4,6,8,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,9,-1,-1,-1,-1,-1,10,-1,
    11,-1,12,-1,13,-1,14,-1,15,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,
    27,-1,28,-1,29,-1,30,-1,31,
    -1,25,-1,-1,-1,-1,-1,26,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,
    23,21,19,17,16,18,20,22,24
};
int weizhi[32] =         //棋子i所在棋盘的位置，走棋需要动态变化
        //weizhi[i] == -1 ---棋子被吃
{
    4,3,5,2,6,1,7,0,8,19,25,27,29,31,33,35,
    85,84,86,83,87,82,88,81,89,64,70,54,56,58,60,62
};
int chescore[32] =
{
    1500,15,15,18,18,80,80,100,100,80,80,20,20,20,20,20,
    1500,15,15,18,18,80,80,100,100,80,80,20,20,20,20,20,

};
//每个棋子可以走的位置
int jiang[18] = {66,67,68,75,76,77,84,85,86,
                3,4,5,12,13,14,21,22,23
               };
int shi[10] = {66,68,76,84,86,
              3,5,13,21,23};
int xiang[14] = {47,51,63,67,71,83,87,
                2,18,38,22,6,42,26
               };
//
int total[400] = {0};
/*
 0----此处无棋子
 1----将
 2----车
 3----炮
 4----马
 5----象
 6----士
 7----兵

*/

void init()
{
    mode = -1;

}
bool findm(int m,int a[],int n)
{
    bool exist = false;
    for(int i = 0;i<n;i++)
    {
        if(a[i] == m)
        {
            exist = true;
            break;
        }
    }
    return exist;
}
int posx(int n,int size)                                //从数组下标获取像素
{
    n=n%16-3;
    n=size*n+size/2+2;
    return n;
}
int posy(int n,int size)
{
    int m=n%16;
    n=(n-m)/16-3;
    n=size*n+size/2+2;
    return n;
}
int getx(int n)
{
    int x = 0;
    x = (n)%9;
    x = x*60+60;
    return x;
}
int gety(int n)
{
    int y = 0;
    y = (n)/9;
    y = y*60 + 60;
    return y;
}
int num(int x,int y,int size)                          //将像素转化为数组下标
{
    int a,b,n;
    if(30<=x&&x<=570&&30<=y&&y<=630)
    {
        x-=size/2;
        y-=size/2;
        a=x%size;
        b=y%size;
        x=(x-a)/size;
        y=(y-b)/size;
        qDebug()<<"x = "<<x;
        qDebug()<<"y = "<<y;
        n=y*9+x;
    }else
        n=0;
    return n;
}