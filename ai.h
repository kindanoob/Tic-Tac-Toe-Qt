#ifndef AI_H
#define AI_H

#include "board.h"
#include "gamestate.h"
#include <QPair>

namespace ai {
QPair<int, int> GetRandomeMove(const Board& board, SideToMove side);
QPair<int, int> GetMinimaxMove(const Board& board, SideToMove side, int depth);
}
#endif // AI_H
