#ifndef CHESSMAN_H
#define CHESSMAN_H
enum _a
{
    shuai,shi,xiang,ma,ju,pao,bing
};

enum _b
{
    red,black
};

class Chessman
{
public:
    Chessman();
    Chessman(int,int,_a,_b,bool selcted=false);
    int x;
    int y;
    _a zi;
    _b rb;
    bool selected;

};

#endif // CHESSMAN_H
