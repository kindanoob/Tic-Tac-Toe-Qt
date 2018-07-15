#ifndef BOARD_H
#define BOARD_H

#include <QVector>

constexpr int kNumRows = 3;
constexpr int kNumCols = 3;
constexpr int kNumSquares = kNumRows * kNumCols;

enum class Piece {
    X,
    O,
    NoPiece
};

class Board {
public:
    Board();
    clear();
    void Reset();
    void PrintToConsole();
    bool CheckRowWin(int row, const Piece& piece);
    bool CheckColWin(int row, const Piece& piece);
    bool CheckMainDiagWin(const Piece& piece);
    bool CheckAntiDiagWin(const Piece& piece);
    bool CheckDraw();
    Piece& At(int row, int col);
private:
    QVector<QVector<Piece>> board;
};

#endif // BOARD_H
