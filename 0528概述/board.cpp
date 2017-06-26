#include "board.h"
#include <QMouseEvent>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QTimer>
#include "ai.h"

Board::Board(QWidget * parent):QWidget(parent), d(Util::d)
{
	ai=new AI(this);
	init();
}

void Board::init()
{

	clearSteps();


	static struct{
		Stone::Type type;
		int row;
		int col;
	} p[16] = 
	{
		{Stone::CHE, 9, 8},
		...
		{Stone::BING, 6, 8}
	};
	
	for(int i = 0; i < 32; ++i)
	{
		s[i].id = i;
		if(i<16)
		{
			s[i].type=p[i].type;
			s[i].row=p[i].row;
			s[i].col=p[i].col;
		}
		else
		{
			s[i].type=p[i-16].type;
			s[i].row=9-p[i-16].row;
			s[i].col=8-p[i-16].col;
		}
		s[i].dead=false;
	}

	selecId=-1;
	redTurn=true;
	ai<init();
}

void Board::save(QString filename)
{
	QFile file(filename);
	file.open(QFile::WriteOnly);

	QDataStream ds(&file);

	ds << this->selectId << this << redTurn;
	for(int i=0; i<32;++i)
	{
		int type = (int)s[i].type;
		ds << s[i].col << s[i].row << s[i].id << s[i].dead << type;
	}

	ds << (int)steps.size();
	foreach(Step* step, steps)
	{
		ds << step->colFrom << step->colTo << step->killid << step->moveid << step->rowFrom << step->rowTo;
	}

	file.close();
}

void Board::load(QString filename)
{
	QFile file(filename);
	file.open(QFile::ReadOnly);

	QDatastream ds(&file);

	ds >> this->selectId >> this->redTurn;
	for(int i=0; i<32; ++i)
	{
		int type;
		ds >> s[i].col >> s[i].row >> s[i].id >> s[i].dead >> type;
		s[i].type=(Stone::Type)type;
	}

	int size;
	ds >> size;
	for(int i=0;i<size;++i)
	{
		Step* step = new Step;
		ds >> step->colFrom >> step->colTo >> step->killid >> step->moveid >> step->rowFrom >> step->rowTo;
		steps.push_back(step);
	}

	file.close();
	update();
}

void Board::backOne()
{
	if(steps.size()==0) return;

	Step * step = steps.last();
	steps.removeLast();

	s[step->moveid].row=step->rowFrom;
	s[step->moveid].col=step->colFrom;
	if(step->killid != -1)
	{
		s[step.killid].dead=false;
	}
	this->redTurn = !this->redTurn;
	selected = -1;

	delete step;
	update();
}

void Board::paintEvent(QPaintEvent *)
{
	QPianter p(this);

	drawPlate(p);

	for(int i = 0; i < 32; ++i)
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

	for(int i=1; i < 9; ++i)
	{
		if(i == 1||i == 9)
		{
			p.drawLine(QPoint(i*d, d), QPoint(i*d, 10*d));
		}
		else
		{
			p.drawLine(QPoint(i*d, d), QPoint(i*d, 5*d));
			p.drawLine(QPoint(i*d, 6*d), QPoint(i*d, 10-*d));
		}
	}

	p.drawLine(QPoint(4*d, 1*d), QPoint(6*d, 3*d));
	...;

	drawCross(p,2,1);
	...;
}

void Board::drawCross(QPainter &p, int row, int col)
{
	QPoint center=Util::getStonePoint(row, col);
	QPoint origin;

	if(col != 8)
	{
		origin = center + QPoint(d/6, d/6);
		p.drawLine(origin, origin+QPoint(0, d/3));
		p.drawLine(origin, origin+QPoint(d/3, 0);
	}
}


































