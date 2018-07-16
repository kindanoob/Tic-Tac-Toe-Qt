#include "ai.h"
#include <cassert>

constexpr int kInfinity = 1e6;

namespace ai {

Move GetRandomeMove(const Board& board, SideToMove side) {
    auto valid_moves = board.GenValidMoves();
    assert(!valid_moves.empty());
    return valid_moves[rand() % valid_moves.size()];
}

Move GetMinimaxMove(SideToMove side, Board& board, int depth) {
    Move best_move;
    int best_score = -kInfinity;
    SideToMove opposite_side = (side == SideToMove::X) ? SideToMove::O : SideToMove::X;
    Piece piece = (side == SideToMove::X) ? Piece::X : Piece::O;
    Piece opposite_piece = (piece == Piece::X) ? Piece::O : Piece::X;
    QVector<Move> valid_moves = board.GenValidMoves();
    assert(!valid_moves.empty());
    for (const Move& curr_move : valid_moves) {
        board.MakeMove(curr_move, piece);
        int curr_score = Minimax(opposite_piece, board, depth - 1, true);
        board.UnmakeMove(curr_move);
        if (curr_score > best_score) {
            best_score = curr_score;
            best_move = curr_move;
        }
    }
    return best_move;
}

int Minimax(Piece piece, Board& board, int depth, bool is_maximizing) {
    if (depth == 0 || board.IsTerminalNode()) {
        return board.EvalBoard();
    }
    int best_score = is_maximizing ? -kInfinity : kInfinity;
    Move best_move;
    Piece opposite_piece = (piece == Piece::X) ? Piece::O : Piece::X;
    QVector<Move> valid_moves = board.GenValidMoves();
    if (is_maximizing) {
        for (const Move& curr_move : valid_moves) {
            board.MakeMove(curr_move, piece);
            int curr_score = Minimax(opposite_piece, board, depth - 1, !is_maximizing);
            board.UnmakeMove(curr_move);
            if (curr_score > best_score) {
                best_score = curr_score;
                best_move = curr_move;
            }
        }
    } else {
        for (const auto& curr_move : valid_moves) {
            board.MakeMove(curr_move, piece);
            int curr_score = Minimax(opposite_piece, board, depth - 1, !is_maximizing);
            board.UnmakeMove(curr_move);
            if (curr_score < best_score) {
                best_score = curr_score;
                best_move = curr_move;
            }
        }
    }
    return best_score;
}

}
