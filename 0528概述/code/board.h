#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include "stone.h"
//#include "ai.h"
class AI;

struct Step
{
    int moveid;
    int killid;
    int rowFrom;
    int colFrom;
    int rowTo;
    int colTo;
};

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);
    void init();
    void save(QString filename);
    void load(QString filename);
    void backOne();

    AI* ai;

    void paintEvent(QPaintEvent *ev);
    void drawPlate(QPainter& p);
    void drawCross(QPainter& p, int row, int col);

    void mousePressEvent(QMouseEvent *ev);
    int getStoneId(int row, int col);

    // 表示选择棋子
    void selectStone(int select);
    // 移动棋子
    void moveStone(int select, int kill, int row, int col);

    bool canMove(int select, int kill, int row, int col);

    bool canMoveJiang(int select, int kill, int row, int col);
    bool canMoveShi(int select, int kill, int row, int col);
    bool canMoveXiang(int select, int kill, int row, int col);
    bool canMoveChe(int select, int kill, int row, int col);
    bool canMoveMa(int select, int kill, int row, int col);
    bool canMovePao(int select, int kill, int row, int col);
    bool canMoveBing(int select, int kill, int row, int col);

    // 必须是一条线，如果两个坐标不是一条线，返回-1
    // 如果是一条线，则返回两个点之间的棋子数量(0, 1, 2)
    int getStoneCount(int row1, int col1, int row2, int col2);

    int selectId; // 如果selectId是-1，那么说明没有棋子被选中
    bool redTurn;

    int& d;

    // 32个棋子
    Stone s[32];
    QList<Step*> steps;
    void addStep(int select, int kill, int row, int col);
    void clearSteps();

signals:

public slots:
};

#endif // BOARD_H
