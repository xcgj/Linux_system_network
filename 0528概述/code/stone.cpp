#include "stone.h"

Stone::Stone() : d(Util::d)
{
    dead = false;
}

void Stone::draw(QPainter &p, int selectId)
{
    // 死掉的棋子不用画
    if(dead) return;

    // 1. 画圆
    // 设置画笔和画刷
    p.setPen(QPen(Qt::black));
    if(id == selectId)
        p.setBrush(Qt::gray);
    else
        p.setBrush(Qt::yellow);

    // 找到中心点，画圆圈
    QPoint center = Util::getStonePoint(row, col);
    p.drawEllipse(center, d/2, d/2);

    // 2. 画字
    QString text = "帅仕相车马炮兵将士象车马炮卒";
    int index = 7 * (id>16) + (int)type;
    QChar ch = text.at(index);

    p.setFont(QFont("宋体", d/2));
    if(id < 16)
        p.setPen(QPen(Qt::red));
    else
        p.setPen(QPen(Qt::black));
    p.drawText(Util::getStoneRect(row, col), Qt::AlignCenter, QString(ch));
}
