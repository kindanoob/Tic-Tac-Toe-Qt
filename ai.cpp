#include "ai.h"
#include <cassert>
#include <QDebug>
#include <algorithm>
#include <chrono>
#include <numeric>

constexpr int kInfinity = 1e6;

namespace ai {

Move GetRandomeMove(SideToMove side, const Board& board) {
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
    std::default_random_engine dre(time(nullptr));
    std::shuffle(valid_moves.begin(), valid_moves.end(), dre);
    assert(!valid_moves.empty());
    for (const Move& curr_move : valid_moves) {
        board.MakeMove(curr_move, piece);
        int curr_score = Minimax(opposite_piece, board, depth - 1, false);
        board.UnmakeMove(curr_move);
        if (curr_score > best_score) {
            best_score = curr_score;
            best_move = curr_move;
        }
    }
    return best_move;
}

int Minimax(Piece piece, Board& board, int depth, bool is_maximizing) {
    Piece opposite_piece = (piece == Piece::X) ? Piece::O : Piece::X;
    if (depth == 0 || board.IsTerminalNode()) {
        int sign = is_maximizing ? -1 : 1;
        // Here we pass opposite_piece to EvalBoard() because if we are in the leaf node with
        // O to move, it means that last move was made by X and it is not possible for O to be
        // a winner of the game.
        return sign * board.EvalBoard(opposite_piece);
    }
    int best_score = is_maximizing ? -kInfinity : kInfinity;
    Move best_move;
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
