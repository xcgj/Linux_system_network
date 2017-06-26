
#include "stone.h"
Stone::Stone():d(Util::d)
{
	dead = false;
}

void Stone::draw(QPainter &p, int selectId)
{
	//死掉的棋子不用 画
	if(dead) return;

	// 1.画圆
	// 设置画笔和画刷
	p.setPen(QPen(Qt::black));
	if(id == selectId)
		p.setBrush(Qt::gray);
	else
		p.setBrush(Qt::yellow);

	QPoint center = Util::getStonePoint(row, col);

	p.drawEllipse(center, d/2, d/2);

	QString text = "帅";
	int index = 7*(id>16)+(int)type;
	QChar ch = text.at(index);

	p.setFont(QFont("111",d/2));
	if (id < 16)
		p.setPen(QPen(Qt::red));
	else
		p.setPen(QPen(Qt::black));
	p.drawText(Util::getStoneRect(row, col), Qt::AlignHCenter, QString(ch));

}
