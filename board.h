#ifndef BOARD_H
#define BOARD_H

#include <QVector>
#include <QPair>

constexpr int kNumRows = 3;
constexpr int kNumCols = 3;
constexpr int kNumSquares = kNumRows * kNumCols;

struct Move {
    Move() : row(-1), col(-1) {}
    Move(int row_, int col_) : row(row_), col(col_) {}
    int row;
    int col;
};

enum class Piece {
    X,
    O,
    NoPiece
};

class Board {
public:
    Board();
//    clear();
    void Reset();
    void PrintToConsole() const;
    bool CheckWin(const Piece& piece) const;
    bool CheckRowWin(int row, const Piece& piece) const;
    bool CheckColWin(int row, const Piece& piece) const;
    bool CheckMainDiagWin(const Piece& piece) const;
    bool CheckAntiDiagWin(const Piece& piece) const;
    bool CheckDraw() const;
    Piece& At(int row, int col);
    QVector<Move> GenValidMoves() const;
    int EvalBoard(Piece piece) const;
    bool IsTerminalNode() const;
    void MakeMove(const Move& move, Piece piece);
    void UnmakeMove(const Move& move);
private:
    QVector<QVector<Piece>> board;
};

#endif // BOARD_H
