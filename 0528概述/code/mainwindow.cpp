#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->board = new Board;
    this->setCentralWidget(board);

    QMenu* file = menuBar()->addMenu("&File");
    file->addAction("新开局", this, SLOT(onNew()));
    file->addAction("保存", this, SLOT(onSave()));
    file->addAction("读盘", this, SLOT(onLoad()));
    file->addAction("悔棋",this, SLOT(onBack()));
    file->addSeparator();
    file->addAction("退出", this, SLOT(close()));
}

void MainWindow::onNew()
{
    board->init();
    board->update();
}

void MainWindow::onSave()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                 "请保存",
                                 "c:\\users\\xueguoliang",
                                 "中国象棋文件 (*.betago)");
    if(filename.length() > 0)
        board->save(filename);
}

void MainWindow::onLoad()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "请选择中国象棋存盘文件",
                                                    "c:\\users\\xueguoliang",
                                                    "中国象棋文件 (*.betago)");
    if(filename.length() > 0)
        board->load(filename);
}

void MainWindow::onBack()
{
    board->backOne();
}
