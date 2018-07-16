#include "ai.h"
#include <cassert>

namespace ai {

QPair<int, int> GetRandomeMove(const Board& board, SideToMove side) {
    auto valid_moves = board.GenValidMoves();
    assert(!valid_moves.empty());
    return valid_moves[rand() % valid_moves.size()];
}

QPair<int, int> GetMinimaxMove(const Board& board, SideToMove side, int depth) {
    return QPair<int, int>();
}

}
