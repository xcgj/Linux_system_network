#ifndef AI_H
#define AI_H

#include "board.h"

class AI
{
public:
    AI(Board* board);
    void init();
    Step* nextStep(int level);



private:
    int getMaxScore(int level, int curMinScore);
    int getMinScore(int level, int curMaxScore);

    void getAllPossibleMove(QList<Step*>& steps);

    void getAllPossibleMoveChe(QList<Step*>& steps, Stone* s);
    void getAllPossibleMoveMa(QList<Step*>& steps, Stone* s);
    void getAllPossibleMovePao(QList<Step*>& steps, Stone* s);
    void getAllPossibleMoveJiang(QList<Step*>& steps, Stone* s);
    void getAllPossibleMoveShi(QList<Step*>& steps, Stone* s);
    void getAllPossibleMoveXiang(QList<Step*>& steps, Stone* s);
    void getAllPossibleMoveBing(QList<Step*>& steps, Stone* s);

    void addStep(QList<Step*>& steps, int moveid, int killid, int row, int col);


    void fakeMove(Step* step);
    void unfakeMove(Step* step);

    int curScore;

    // 红方局面分
   // int getRedScore();
    // 黑方局面分
  //  int getBlackScore();

    // AI要得到局面信息，用于判断下一步
    Board* board;
};

#endif // AI_H
