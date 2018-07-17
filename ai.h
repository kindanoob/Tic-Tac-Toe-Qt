#ifndef AI_H
#define AI_H

#include "board.h"
#include "gamestate.h"
#include <QPair>

namespace ai {
constexpr int kDefaultMinimaxDepth = 10;

Move GetRandomeMove(SideToMove side, const Board& board);
Move GetMinimaxMove(SideToMove side, Board& board, int depth);
int Minimax(Piece piece, Board& board, int depth, bool is_maximizing);
}
#endif // AI_H
