#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "board.h"

// 提供窗口框架
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    // 棋盘是MainWindow的中心窗口CentralWidget
    Board* board;

signals:

public slots:
    void onNew();
    void onSave();
    void onLoad();
    void onBack();
};

#endif // MAINWINDOW_H
