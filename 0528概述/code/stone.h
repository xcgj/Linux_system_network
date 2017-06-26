#ifndef STONE_H
#define STONE_H

#include <QPainter>
#include "util.h"

class Stone
{
public:
    Stone();

    enum Type {JIANG, SHI, XIANG, CHE, MA, PAO, BING};

    int row;
    int col;
    Type type;
    int id; // 0~15 红色 16～31黑色
    bool dead;
    int& d;

    void draw(QPainter& p, int selectId);
};

#endif // STONE_H
