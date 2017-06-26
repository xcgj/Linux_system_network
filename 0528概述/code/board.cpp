#include "board.h"
#include <QMouseEvent>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QTimer>
#include "ai.h"

Board::Board(QWidget *parent) : QWidget(parent), d(Util::d)
{
    ai = new AI(this);
    init();
}

void Board::init()
{
    // 清除步骤信息
    clearSteps();

    // 棋子初始位置表
    static struct {
        Stone::Type type;
        int row;
        int col;
    } p[16] =
    {
    {Stone::CHE, 9, 8},
    {Stone::MA, 9, 7},
    {Stone::XIANG, 9, 6},
    {Stone::SHI, 9, 5},
    {Stone::JIANG, 9, 4},
    {Stone::CHE, 9, 0},
    {Stone::MA, 9, 1},
    {Stone::XIANG, 9, 2},
    {Stone::SHI, 9, 3},
    {Stone::PAO, 7, 1},
    {Stone::PAO, 7, 7},
    {Stone::BING, 6, 0},
    {Stone::BING, 6, 2},
    {Stone::BING, 6, 4},
    {Stone::BING, 6, 6},
    {Stone::BING, 6, 8}
};

    // 初始化32个棋子
    for(int i=0; i<32; ++i)
    {
        s[i].id = i;
        if(i<16)
        {
            s[i].type = p[i].type;
            s[i].row = p[i].row;
            s[i].col = p[i].col;
        }
        else
        {
            s[i].type = p[i-16].type;
            s[i].row = 9 - p[i-16].row;
            s[i].col = 8 - p[i-16].col;
        }
        s[i].dead = false;
    }

    // 棋局中间状态变量
    selectId = -1;
    redTurn = true;
    ai->init();
}

void Board::save(QString filename)
{
    QFile file(filename);
    file.open(QFile::WriteOnly);

    QDataStream ds(&file);

    ds << this->selectId << this->redTurn;
    for(int i=0; i<32; ++i)
    {
        int type = (int)s[i].type;
        ds << s[i].col << s[i].row << s[i].id << s[i].dead << type;
    }

    ds << (int)steps.size();
    foreach(Step* step, steps)
    {
        ds << step->colFrom << step->colTo << step->killid << step->moveid
           << step->rowFrom << step->rowTo;
    }

    file.close();
}

void Board::load(QString filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);

    QDataStream ds(&file);

    ds >> this->selectId >> this->redTurn;
    for(int i=0; i<32; ++i)
    {
        int type;
        ds >> s[i].col >> s[i].row >> s[i].id >> s[i].dead >> type;
        s[i].type = (Stone::Type)type;
    }

    int size;
    ds >> size;
    for(int i=0; i<size; ++i)
    {
        Step* step = new Step;
        ds >> step->colFrom >> step->colTo >> step->killid >> step->moveid
           >> step->rowFrom >> step->rowTo;
        steps.push_back(step);
    }

    file.close();
    update();
}

void Board::backOne()
{
    if(steps.size() == 0)
        return;

    Step* step = steps.last();
    steps.removeLast();

    s[step->moveid].row = step->rowFrom;
    s[step->moveid].col = step->colFrom;
    if(step->killid != -1)
        s[step->killid].dead = false;

    this->redTurn = !this->redTurn;
    selectId = -1;

    delete step;
    update();
}

void Board::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    // 棋盘
    drawPlate(p);

    // 棋子
    for(int i=0; i<32; ++i)
    {
        s[i].draw(p, selectId);
    }
}

void Board::drawPlate(QPainter &p)
{
    for(int i=1; i<=10; ++i)
    {
        p.drawLine(QPoint(d, i*d), QPoint(9*d, i*d));
    }
    for(int i=1; i<=9; ++i)
    {
        if(i==1 || i==9)
        {
            p.drawLine(QPoint(i*d, d), QPoint(i*d, 10*d));
        }
        else
        {
            p.drawLine(QPoint(i*d, d), QPoint(i*d, 5*d));
            p.drawLine(QPoint(i*d, 6*d), QPoint(i*d, 10*d));
        }
    }

    // 九宫格
    p.drawLine(QPoint(4*d, 1*d), QPoint(6*d, 3*d));
    p.drawLine(QPoint(4*d, 3*d), QPoint(6*d, 1*d));

    p.drawLine(QPoint(4*d, 8*d), QPoint(6*d, 10*d));
    p.drawLine(QPoint(6*d, 8*d), QPoint(4*d, 10*d));

    drawCross(p, 2, 1);
    drawCross(p, 2, 7);
    drawCross(p, 3, 0);
    drawCross(p, 3, 2);
    drawCross(p, 3, 4);
    drawCross(p, 3, 6);
    drawCross(p, 3, 8);

    drawCross(p, 6, 0);
    drawCross(p, 6, 2);
    drawCross(p, 6, 4);
    drawCross(p, 6, 6);
    drawCross(p, 6, 8);
    drawCross(p, 7, 1);
    drawCross(p, 7, 7);
}

void Board::drawCross(QPainter &p, int row, int col)
{
    QPoint center = Util::getStonePoint(row, col);
    QPoint origin;
    if(col != 8)
    {
        // 右下角
        origin = center + QPoint(d/6, d/6);
        p.drawLine(origin, origin+QPoint(0, d/3));
        p.drawLine(origin, origin+QPoint(d/3, 0));

        // 右上角
        origin = center + QPoint(d/6, -d/6);
        p.drawLine(origin, origin+QPoint(0, -d/3));
        p.drawLine(origin, origin+QPoint(d/3, 0));
    }

    if(col!=0)
    {
        // 坐上角
        origin = center + QPoint(-d/6, -d/6);
        p.drawLine(origin, origin+QPoint(0, -d/3));
        p.drawLine(origin, origin+QPoint(-d/3, 0));

        // 左下角
        origin = center + QPoint(-d/6, d/6);
        p.drawLine(origin, origin+QPoint(0, d/3));
        p.drawLine(origin, origin+QPoint(-d/3, 0));
    }
}

void Board::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::RightButton) return;

    // 棋盘像素坐标 --> 棋盘坐标
    QPoint ptClick = ev->pos();
    int row=-1, col=-1;
    if(!Util::getPlateCood(ptClick, row, col))
    {
        // 点到棋盘外的画，此次点击作废
        return;
    }
    qDebug() << row << col;

    // 判断一下这个上面是不是有棋子
    int clickId = getStoneId(row, col);

    // 判断状态：选择，走棋
    if(selectId == -1)
    {
        selectStone(clickId);
    }
    else
    {
        // 避免相互残杀
        if(clickId != -1 && (clickId <16 ==  selectId<16))
        {
            selectStone(clickId);
            return;
        }

        if(canMove(selectId, clickId, row, col))
            moveStone(selectId, clickId, row, col);
    }
}

int Board::getStoneId(int row, int col)
{
    for(int i=0; i<32; ++i)
    {
        if(s[i].row == row && s[i].col == col && !s[i].dead)
        {
            return i;
        }
    }
    return -1;
}

void Board::selectStone(int select)
{
    if(redTurn != select < 16) return;

    // select
    selectId = select;
    update();
}

void Board::moveStone(int select, int kill, int row, int col)
{
    // 记录步骤
    addStep(select, kill, row, col);

    // move
    s[select].row = row;
    s[select].col = col;
    if(kill != -1)
        s[kill].dead = true;

    selectId = -1;
    redTurn = !redTurn;

    update();

    if(!this->redTurn)
    {
        QTimer::singleShot(10, [&](){
            // 该电脑走了
            Step* step = ai->nextStep(2);
            moveStone(step->moveid, step->killid, step->rowTo, step->colTo);
            delete step;
        });
    }
}

/* 复杂问题 */
bool Board::canMove(int select, int kill, int row, int col)
{
    switch(s[select].type)
    {
    case Stone::JIANG:
        return canMoveJiang(select, kill, row, col);
        break;
    case Stone::SHI:
        return canMoveShi(select, kill, row, col);
        break;
    case Stone::XIANG:
        return canMoveXiang(select, kill, row, col);
        break;
    case Stone::CHE:
        return canMoveChe(select, kill, row, col);
        break;
    case Stone::MA:
        return canMoveMa(select, kill, row, col);
        break;
    case Stone::PAO:
        return canMovePao(select, kill, row, col);
        break;
    case Stone::BING:
        return canMoveBing(select, kill, row, col);
        break;
    }
    return true;
}

/*
1. 只能走一步
2. 目标只能在九宫内
3. 将照面可以直接吃
*/
bool Board::canMoveJiang(int select, int kill, int row, int col)
{
    // 照面跟车是一样的
    if(kill != -1 && s[kill].type == Stone::JIANG)
    {
        int ret = getStoneCount(s[select].row, s[select].col, row, col);
        if(ret == 0)
            return true;
    }

    // 解决只能走一步问题
    int dRow = qAbs(s[select].row - row);
    int dCol = qAbs(s[select].col - col);
    if(dRow + dCol != 1)
        return false;

    // 目标只能在九宫内
    if(col < 3 || col > 5) return false;
    if(s[select].id < 16)
    {
        if(row < 7 || row > 9) return false;
    }
    else
    {
        if(row < 0 || row > 2) return false;
    }



    return true;
}

/*
1. 斜着走
2. 目标在九宫内
*/
bool Board::canMoveShi(int select, int , int row, int col)
{
    // 目标只能在九宫内
    if(col < 3 || col > 5) return false;
    if(s[select].id < 16)
    {
        if(row < 7 || row > 9) return false;
    }
    else
    {
        if(row < 0 || row > 2) return false;
    }

    // 斜着走
    int dRow = qAbs(s[select].row - row);
    int dCol = qAbs(s[select].col - col);
    if(dRow * 10 + dCol != 11)
        return false;

    return true;
}

/*
1. 相不能过河
2. 相飞田
3. 象眼不能有子
*/
bool Board::canMoveXiang(int select, int , int row, int col)
{
    // 不能过河
    if(s[select].id < 16)
    {
        if(row < 5) return false;
    }
    else
    {
        if(row > 4) return false;
    }

    // 相飞田
    int dRow = qAbs(s[select].row - row);
    int dCol = qAbs(s[select].col - col);
    if(dRow * 10 + dCol != 22)
        return false;

    // 象眼不能有子
    int eRow = (s[select].row + row)/2;
    int eCol = (s[select].col + col)/2;
    int eye = getStoneId(eRow, eCol);
    if(eye != -1)
        return false;
    return true;
}

bool Board::canMoveChe(int select, int , int row, int col)
{
    int ret = getStoneCount(s[select].row, s[select].col, row, col);
    if(ret != 0) return false;
    return true;
}

/*
1. 马走日
2. 不能碍马腿
*/
bool Board::canMoveMa(int select, int , int row, int col)
{
    // 马走日
    int dRow = qAbs(s[select].row - row);
    int dCol = qAbs(s[select].col - col);
    int d = dRow * 10 + dCol;
    if(d != 21 && d != 12)
        return false;

    // 马腿
    if(d == 21)
    {
        int eRow = (s[select].row + row)/2;
        int eCol = s[select].col;
        if(getStoneId(eRow, eCol) != -1) return false;
    }
    else
    {
        int eRow = s[select].row;
        int eCol = (s[select].col + col)/2;
        if(getStoneId(eRow, eCol) != -1) return false;
    }
    return true;
}

bool Board::canMovePao(int select, int kill, int row, int col)
{
    if(kill == -1)
    {
        int ret = getStoneCount(s[select].row, s[select].col, row, col);
        if(ret != 0) return false;
    }
    else
    {
        int ret = getStoneCount(s[select].row, s[select].col, row, col);
        if(ret != 1) return false;
    }
    return true;
}

/*
1. 只能走一步
2. 过河前不能后退，不能平移
3. 过河后可以平移
*/
bool Board::canMoveBing(int select, int , int row, int col)
{
    // 解决只能走一步问题
    int dRow = qAbs(s[select].row - row);
    int dCol = qAbs(s[select].col - col);
    if(dRow + dCol != 1)
        return false;

    // 红色
    if(s[select].id < 16)
    {
        // 在后退
        if(row > s[select].row) return false;

        // 没过河
        if(s[select].row >= 5)
        {
            if(row == s[select].row ) return false;
        }
    }
    else
    {
        // 在后退
        if(row < s[select].row) return false;

        // 没过河
        if(s[select].row <= 4)
        {
            if(row == s[select].row ) return false;
        }
    }
    return true;
}

int Board::getStoneCount(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    if(row1 != row2 && col1!=col2) return -1;

    if(row1 == row2)
    {
        if(col1 > col2)
        {
            int tmp = col1;
            col1 = col2;
            col2 = tmp;
        }
        for(int col = col1+1; col < col2; ++col)
        {
            if(getStoneId(row1, col) != -1) ret++;
        }
    }
    else // col 相等
    {
        if(row1 > row2)
        {
            int tmp = row1;
            row1 =row2;
            row2 = tmp;
        }
        for(int row = row1+1; row < row2; ++row)
        {
            if(getStoneId(row, col1) != -1) ret++;
        }
    }

    return ret;
}

void Board::addStep(int select, int kill, int row, int col)
{
    Step* step = new Step;
    step->moveid = select;
    step->killid = kill;
    step->rowTo = row;
    step->colTo = col;
    step->rowFrom = s[select].row;
    step->colFrom = s[select].col;
    steps.append(step);
}

void Board::clearSteps()
{

    while(steps.size())
    {
        Step* step = steps.first();
        delete step;
        steps.removeFirst();
    }

}

