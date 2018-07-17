#include "board.h"
#include <QDebug>

constexpr int kWinEval = 100;
constexpr int kDrawEval = 0;

Board::Board()
{
    for (int row = 0; row < kNumRows; ++row) {
        board.append(QVector<Piece>());
        for (int col = 0; col < kNumCols; ++col) {
            board[row].append(Piece::NoPiece);
        }
    }
}

void Board::Reset() {
    for (auto& row : board) {
        for (auto& square : row) {
            square = Piece::NoPiece;
        }
    }
}

void Board::PrintToConsole() const {
    for (int row = 0; row < kNumRows; ++row) {
        QString curr_row;
        for (int col = 0; col < kNumCols; ++col) {
            QString curr("_");
            if (board[row][col] == Piece::X) {
                curr = "X";
            } else if (board[row][col] == Piece::O) {
                curr = "O";
            }
            curr_row += curr;
        }
        qDebug() << curr_row;
    }
}

Piece& Board::At(int row, int col) {
    assert(row >= 0 && row < kNumRows && col >= 0 && col < kNumCols);
    return board[row][col];
}

bool Board::CheckWin(const Piece& piece) const {
    assert(piece != Piece::NoPiece);
    for (int i = 0; i < kNumRows; ++i) {
        if (CheckRowWin(i, piece) || CheckColWin(i, piece)) {
            return true;
        }
    }
    return CheckMainDiagWin(piece) || CheckAntiDiagWin(piece);
}

bool Board::CheckRowWin(int row, const Piece& piece) const {
    if (board[row][0] != piece) {
        return false;
    }
    for (int col = 0; col < kNumCols - 1; ++col) {
        if (board[row][col] != board[row][col + 1]) {
            return false;
        }
    }
    return true;
}

bool Board::CheckColWin(int col, const Piece& piece) const {
    if (board[0][col] != piece) {
        return false;
    }
    for (int row = 0; row < kNumRows - 1; ++row) {
        if (board[row][col] != board[row + 1][col]) {
            return false;
        }
    }
    return true;
}

bool Board::CheckMainDiagWin(const Piece& piece) const {
    for (int row = 0; row < kNumRows; ++row) {
        if (board[row][row] != piece) {
            return false;
        }
    }
    return true;
}

bool Board::CheckAntiDiagWin(const Piece& piece) const {
    for (int row = 0; row < kNumRows; ++row) {
        if (board[row][kNumRows - row - 1] != piece) {
            return false;
        }
    }
    return true;
}

bool Board::CheckDraw() const {
    int piece_cnt = 0;
    for (int row = 0; row < kNumRows; ++row) {
        for (int col = 0; col < kNumCols; ++col) {
            if (board[row][col] != Piece::NoPiece) {
                ++piece_cnt;
            }
        }
    }
    return piece_cnt == kNumSquares;
}

QVector<Move> Board::GenValidMoves() const {
    QVector<Move> valid_moves;
    for (int row = 0; row < kNumRows; ++row) {
        for (int col = 0; col < kNumCols; ++col) {
            if (board[row][col] == Piece::NoPiece) {
                valid_moves.append(Move(row, col));
            }
        }
    }
    return valid_moves;
}

int Board::EvalBoard(Piece piece) const {
    if (CheckWin(piece)) {
        return kWinEval;
    }
    if (CheckDraw()) {
        return kDrawEval;
    }
    // This piece of code should be reached only if we reached depth == 0 in
    // the minimax call and the game is not finished. It can happen, for instance,
    // when the kDefaultMinimaxDepth parameter is low. Since the tree of all
    // possible positions in Tic-Tac-Toe is rather small, we just set the default
    // minimax depth to 10, so that every time we call EvalBoard() the game is
    // already finished.
    assert(false);
    return 0;
}

bool Board::IsTerminalNode() const {
    return CheckWin(Piece::X) || CheckWin(Piece::O) || CheckDraw();
}

void Board::MakeMove(const Move& move, Piece piece) {
    board[move.row][move.col] = piece;
}

void Board::UnmakeMove(const Move& move) {
    board[move.row][move.col] = Piece::NoPiece;
}
