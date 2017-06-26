#include "ai.h"

static int ss[7] = {10000, 100, 100, 500, 250, 250, 100};
static int INFINITE = 1000000;

AI::AI(Board* board) : board(board)
{
    curScore = 0;
}

void AI::init()
{
    curScore = 0; // black - red == 0
}

int AI::getMaxScore(int level, int curMinScore)
{
    if(level == 0)
        return this->curScore;
        //return getBlackScore() - getRedScore();

    QList<Step*> steps;
    getAllPossibleMove(steps);

    int maxScore = -INFINITE;
    foreach(Step* step, steps)
    {
        fakeMove(step);
        int score = getMinScore(level-1, maxScore);// getBlackScore() - getRedScore();
        unfakeMove(step);

        // 剪枝优化
        if(score >= curMinScore)
        {
            maxScore = score;
            break;
        }
        if(score > maxScore)
            maxScore = score;
    }

    foreach(Step* step, steps)
    {
        delete step;
    }
    steps.clear();
    return maxScore;
}

// 对方走一步之后，我的最惨结果
int AI::getMinScore(int level, int curMaxScore)
{
    if(level == 0)
        return curScore;
       // return getBlackScore() - getRedScore();

    QList<Step*> steps;
    getAllPossibleMove(steps);

    int minScore = INFINITE;
    foreach(Step* step, steps)
    {
        fakeMove(step);
        int score = getMaxScore(level-1, minScore);//getBlackScore() - getRedScore();
        unfakeMove(step);

        // 剪枝优化 发现当前的分数，还不如以前其他步骤的分数高，那么这个分支其他局面不需要再考虑
        if(score <= curMaxScore)
        {
            minScore = score;
            break;
        }

        if(score < minScore)
            minScore = score;
    }

    foreach(Step* step, steps)
    {
        delete step;
    }
    steps.clear();

    return minScore;
}

Step *AI::nextStep(int level)
{
    // 得到所有可能的走棋方式
    QList<Step*> steps;
    getAllPossibleMove(steps);

    Step* select = NULL;
    int maxScore = -INFINITE;

    foreach(Step* step, steps)
    {
        // 假如使用step来移动棋子
        fakeMove(step);
        // 计算局面分
        int score = getMinScore(level-1, maxScore);//getBlackScore() - getRedScore();
        // 将局面恢复
        unfakeMove(step);

        if(score > maxScore)
        {
            maxScore = score;
            select = step;
        }
    }

    // 清除内存
    foreach (Step* step, steps) {
        if(step != select) delete step;
    }
    steps.clear();

    if(select->killid != -1)
    {
#if 0
        if(select->killid > 15)
        {
            curScore -= ss[select->killid];
        }
        else
        {
            curScore += ss[select->killid]; // 只有这个分支有效
        }
#endif
        curScore += ss[select->killid];
    }

    return select;
}



void AI::getAllPossibleMove(QList<Step *> &steps)
{
    int min = 0;
    int max = 16;
    if(!board->redTurn)
    {
        min = 16, max = 32;
    }
    for(int i=min; i<max; ++i)
    {
        if(board->s[i].dead) continue;

        switch (board->s[i].type) {
        case Stone::CHE:
            getAllPossibleMoveChe(steps, &board->s[i]);
            break;
        case Stone::MA:
            getAllPossibleMoveMa(steps, &board->s[i]);
            break;
        case Stone::PAO:
            getAllPossibleMovePao(steps, &board->s[i]);
            break;
        case Stone::BING:
            getAllPossibleMoveBing(steps, &board->s[i]);
            break;
        case Stone::JIANG:
            getAllPossibleMoveJiang(steps, &board->s[i]);
            break;
        case Stone::SHI:
            getAllPossibleMoveShi(steps, &board->s[i]);
            break;
        case Stone::XIANG:
            getAllPossibleMoveXiang(steps, &board->s[i]);
            break;
        default:
            break;
        }
#if 0
        for(int row=0; row<10; ++row)
        {
            for(int col=0; col<9; ++col)
            {
                int id = board->getStoneId(row, col);
                if(id != -1 && Util::color(id) == Util::color(i))
                {
                    continue;
                }
                if(board->canMove(i, id, row, col))
                {
                    Step* step = new Step;
                    step->moveid = i;
                    step->killid = id;
                    step->rowTo = row;
                    step->colTo = col;
                    step->rowFrom = board->s[i].row;
                    step->colFrom = board->s[i].col;
                    steps.push_back(step);
                }
            }
        }
#endif
    }
}

void AI::getAllPossibleMoveChe(QList<Step *> &steps, Stone* s)
{
    int row = s->row;
    int col = s->col;

    for(int iRow = row-1; iRow >= 0; iRow --)
    {
        int id = board->getStoneId(iRow, col);
        if(id != -1)
        {
            if(Util::color(id) != Util::color(s->id))
            {
                addStep(steps, s->id, id, iRow, col);
            }
            break;
        }

        addStep(steps, s->id, -1, iRow, col);
    }

    for(int iRow = row+1; iRow <= 9; iRow ++)
    {
        int id = board->getStoneId(iRow, col);
        if(id != -1)
        {
            if(Util::color(id) != Util::color(s->id))
            {
                addStep(steps, s->id, id, iRow, col);
            }
            break;
        }

        addStep(steps, s->id, -1, iRow, col);
    }

    for(int iCol = col-1; iCol >= 0; iCol --)
    {
        int id = board->getStoneId(row, iCol);
        if(id != -1)
        {
            if(Util::color(id) != Util::color(s->id))
            {
                addStep(steps, s->id, id, row, iCol);
            }
            break;
        }
        addStep(steps, s->id, -1, row, iCol);
    }

    for(int iCol = col+1; iCol <= 8; ++iCol)
    {
        int id = board->getStoneId(row, iCol);
        if(id != -1)
        {
            if(Util::color(id) != Util::color(s->id))
            {
                addStep(steps, s->id, id, row, iCol);
            }
            break;
        }
        addStep(steps, s->id, -1, row, iCol);
    }

}

void AI::getAllPossibleMoveMa(QList<Step *> &steps, Stone* s)
{
    int row = s->row;
    int col = s->col;

    struct {
        int dx;
        int dy;
    } offs[8] =
    {
    {1, 2},
    {2, 1},
    {2, -1},
    {-2, 1},
    {-1, -2},
    {-2, -1},
    {-1, 2},
    {1, -2}
    };

    for(int i=0; i<8; ++i)
    {
        int iRow = row + offs[i].dx;
        int iCol = col + offs[i].dy;
        if(iRow < 0 || iRow > 9) continue;
        if(iCol < 0 || iCol > 8) continue;

        int id = board->getStoneId(iRow, iCol);
        if(id != -1 && Util::color(id) == Util::color(s->id))
        {
            continue;
        }

        if(board->canMove(s->id, id, iRow, iCol))
        {
            addStep(steps, s->id, id, iRow, iCol);
        }
    }
}

void AI::getAllPossibleMovePao(QList<Step *> &steps, Stone* s)
{
    int row = s->row;
    int col = s->col;

    for(int iRow = 0; iRow <= 9; ++iRow)
    {
        int id = board->getStoneId(iRow, col);
        if(id != -1 && Util::color(s->id) == Util::color(id))
            continue;
        if(board->canMove(s->id, id, iRow, col))
            addStep(steps, s->id, id, iRow, col);
    }

    for(int iCol = 0; iCol <= 8; ++iCol)
    {
        int id = board->getStoneId(row, iCol);
        if(id != -1 && Util::color(s->id) == Util::color(id))
            continue;
        if(board->canMove(s->id, id, row, iCol))
            addStep(steps, s->id, id, row, iCol);
    }
}

void AI::getAllPossibleMoveJiang(QList<Step *> &steps, Stone* s)
{
    int row = s->row;
    int col = s->col;
    struct {
        int dx;
        int dy;
    } offs[4] =
    {
    {1, 0},
    {0, 1},
    {-1, 0},
    {0, -1},
    };

    for(int i=0; i<4; ++i)
    {
        int iRow = row + offs[i].dx;
        int iCol = col + offs[i].dy;
        if(iRow < 0 || iRow > 9) continue;
        if(iCol < 0 || iCol > 8) continue;

        int id = board->getStoneId(iRow, iCol);
        if(id != -1 && Util::color(id) == Util::color(s->id))
        {
            continue;
        }

        if(board->canMove(s->id, id, iRow, iCol))
        {
            addStep(steps, s->id, id, iRow, iCol);
        }
    }

    // 判断是否两个将照面
    int peerJiang;
    if(s->id > 16)  peerJiang = s->id - 16;
    else            peerJiang = s->id + 16;

    if(board->canMove(s->id, peerJiang,
                      board->s[peerJiang].row,
                      board->s[peerJiang].col))
    {
        addStep(steps, s->id, peerJiang,
                board->s[peerJiang].row,
                board->s[peerJiang].col);
    }
}

void AI::getAllPossibleMoveShi(QList<Step *> &steps, Stone* s)
{
    int row = s->row;
    int col = s->col;
    struct {
        int dx;
        int dy;
    } offs[4] =
    {
    {1, 1},
    {-1, 1},
    {1, -1},
    {-1, -1},
    };

    for(int i=0; i<4; ++i)
    {
        int iRow = row + offs[i].dx;
        int iCol = col + offs[i].dy;
        if(iRow < 0 || iRow > 9) continue;
        if(iCol < 0 || iCol > 8) continue;

        int id = board->getStoneId(iRow, iCol);
        if(id != -1 && Util::color(id) == Util::color(s->id))
        {
            continue;
        }

        if(board->canMove(s->id, id, iRow, iCol))
        {
            addStep(steps, s->id, id, iRow, iCol);
        }
    }
}

void AI::getAllPossibleMoveXiang(QList<Step *> &steps, Stone* s)
{
    int row = s->row;
    int col = s->col;
    struct {
        int dx;
        int dy;
    } offs[4] =
    {
    {2, 2},
    {-2, 2},
    {2, -2},
    {-2, -2},
    };

    for(int i=0; i<4; ++i)
    {
        int iRow = row + offs[i].dx;
        int iCol = col + offs[i].dy;
        if(iRow < 0 || iRow > 9) continue;
        if(iCol < 0 || iCol > 8) continue;

        int id = board->getStoneId(iRow, iCol);
        if(id != -1 && Util::color(id) == Util::color(s->id))
        {
            continue;
        }

        if(board->canMove(s->id, id, iRow, iCol))
        {
            addStep(steps, s->id, id, iRow, iCol);
        }
    }
}

void AI::getAllPossibleMoveBing(QList<Step *> &steps, Stone* s)
{
    int row = s->row;
    int col = s->col;
    struct {
        int dx;
        int dy;
    } offs[4] =
    {
    {1, 0},
    {0, 1},
    {-1, 0},
    {0, -1},
    };

    for(int i=0; i<4; ++i)
    {
        int iRow = row + offs[i].dx;
        int iCol = col + offs[i].dy;
        if(iRow < 0 || iRow > 9) continue;
        if(iCol < 0 || iCol > 8) continue;

        int id = board->getStoneId(iRow, iCol);
        if(id != -1 && Util::color(id) == Util::color(s->id))
        {
            continue;
        }

        if(board->canMove(s->id, id, iRow, iCol))
        {
            addStep(steps, s->id, id, iRow, iCol);
        }
    }
}

void AI::addStep(QList<Step *> &steps, int moveid, int killid, int row, int col)
{
    Step* step = new Step;
    step->moveid = moveid;
    step->killid = killid;
    step->rowTo = row;
    step->colTo = col;
    step->rowFrom = board->s[moveid].row;
    step->colFrom = board->s[moveid].col;
    steps.push_back(step);
}

void AI::fakeMove(Step *step)
{
    // 将moveStone当中的修改数据部分，但是不更新屏幕
    board->s[step->moveid].row = step->rowTo;
    board->s[step->moveid].col = step->colTo;
    if(step->killid != -1)
    {
        // 黑色的棋子被干掉了，所以分数要下降
        if(!Util::color(step->killid))
        {
            curScore -= ss[step->killid];
        }
        else
        {
            curScore += ss[step->killid];
        }
        board->s[step->killid].dead = true;
    }
    board->redTurn = !board->redTurn;
}

void AI::unfakeMove(Step *step)
{
    // 将backOne函数的修改数据部分代码，但是不更新屏幕
    board->s[step->moveid].row = step->rowFrom;
    board->s[step->moveid].col = step->colFrom;
    if(step->killid != -1)
    {
        if(!Util::color(step->killid))
        {
            curScore += ss[step->killid];
        }
        else
        {
            curScore -= ss[step->killid];
        }
        board->s[step->killid].dead = false;
    }

    board->redTurn = !board->redTurn;
}


#if 0
int AI::getRedScore()
{
    int score = 0;
    // 把所有红色的棋子的值相加
    for(int i=0; i<16; ++i)
    {
        if(board->s[i].dead) continue;
        score += ss[board->s[i].type];
    }

    return score;
}

int AI::getBlackScore()
{
    int score = 0;
    for(int i=16; i<32; ++i)
    {
        if(board->s[i].dead) continue;
        score += ss[board->s[i].type];
    }
    return score;
}
#endif
