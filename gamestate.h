#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "board.h"
#include <QVector>
#include <QRect>

enum class SideToMove {
    X,
    O
};

enum class GameStatus {
    InProgress,
    XWon,
    OWon,
    Draw
};

enum class Player {
    Human,
    Computer
};

class GameState
{
public:
    GameState();
    void Reset();
    void ResetBoard();
    void ResetSideToMove();
    void ResetPlayerToMove();
    void ResetGameStatus();
    void ResetIsFinished();

    bool IsGameFinished();
    bool CheckWin(const SideToMove& side);
    bool CheckDraw();
    bool CheckRowWin(int row, const SideToMove& side);
    bool CheckColWin(int col, const SideToMove& side);
    bool CheckMainDiagWin(const SideToMove& side);
    bool CheckAntiDiagWin(const SideToMove& side);

    //setters and getters
    void SetSideToMove(const SideToMove& side);
    const SideToMove GetSideToMove() const;
    void SwitchSideToMove();
    const bool GetIsFinished() const;
    void SetIsFinished(bool b);
        const GameStatus GetGameStatus() const;
    void SetGameStatus(const GameStatus& status);
    void UpdateGameStatus();
    Board& GetBoard();
    const Board& GetBoard() const;
    const Piece GetPieceToMove() const;
    Player GetPlayerToMove() const;
    void SetPlayerToMove(Player player);
    void SwitchPlayerToMove();

    void MakeMove(const Move& move);

    QString GetGameOutcomeText() const;
protected:
    //
private:
    Board board;
    SideToMove side_to_move;
    bool is_finished;
    GameStatus game_status;
    Player PlayerX;
    Player PlayerO;
    Player player_to_move;
};

#endif // GAMESTATE_H
