#include "gamestate.h"
#include <QDebug>

GameState::GameState() :
    side_to_move(SideToMove::X),
    is_finished(false),
    game_status(GameStatus::InProgress),
    player_x(Player::Human),
    player_o(Player::Computer),
    player_to_move(Player::Human),
    computer_mode(ComputerMode::kPlaysO),
    ai_algorithm(AiAlgorithm::kRandom)
{

}

void GameState::SetSideToMove(const SideToMove& side) {
    side_to_move = side;
}

const SideToMove GameState::GetSideToMove() const {
    return side_to_move;
}

void GameState::SwitchSideToMove() {
    if (side_to_move == SideToMove::X) {
        SetSideToMove(SideToMove::O);
    } else {
        SetSideToMove(SideToMove::X);
    }
}

bool GameState::IsGameFinished() {
    return game_status != GameStatus::InProgress;
}

void GameState::UpdateGameStatus() {
    if (CheckWin(SideToMove::X)) {
        game_status = GameStatus::XWon;
        is_finished = true;
    } else if (CheckWin(SideToMove::O)) {
        game_status = GameStatus::OWon;
        is_finished = true;
    } else if (CheckDraw()) {
        game_status = GameStatus::Draw;
        is_finished = true;
    }
}

bool GameState::CheckWin(const SideToMove& side) {
    Piece piece = side == SideToMove::X ? Piece::X : Piece::O;
    for (int i = 0; i < kNumRows; ++i) {
        if (board.CheckRowWin(i, piece) || board.CheckColWin(i, piece)) {
            return true;
        }
    }
    return board.CheckMainDiagWin(piece) || board.CheckAntiDiagWin(piece);
}

bool GameState::CheckDraw() {
    return board.CheckDraw();
}

void GameState::Reset() {
    board.Reset();
    ResetSideToMove();
    ResetPlayerToMove();
    ResetGameStatus();
    ResetIsFinished();
}

void GameState::ResetSideToMove() {
    SetSideToMove(SideToMove::X);
}

void GameState::ResetPlayerToMove() {
    SetPlayerToMove(GetPlayerX());
}

void GameState::ResetGameStatus() {
    SetGameStatus(GameStatus::InProgress);
}

void GameState::ResetIsFinished() {
    SetIsFinished(false);
}

void GameState::SetIsFinished(bool b) {
    is_finished = b;
}

const bool GameState::GetIsFinished() const {
    return is_finished;
}

void GameState::SetGameStatus(const GameStatus &status) {
    game_status = status;
}

const GameStatus GameState::GetGameStatus() const {
    return game_status;
}

Board& GameState::GetBoard() {
    return board;
}

const Board& GameState::GetBoard() const {
    return board;
}

const Piece GameState::GetPieceToMove() const {
    return GetSideToMove() == SideToMove::X ? Piece::X : Piece::O;
}

QString GameState::GetGameOutcomeText() const {
    assert(GetIsFinished());
    QString outcome("Game is finished.\n");
    if (GetGameStatus() == GameStatus::XWon) {
        outcome += "X Won.";
    } else if (GetGameStatus() == GameStatus::OWon) {
        outcome += "O Won.";
    } else {
        outcome += "Draw.";
    }
    return outcome;
}

Player GameState::GetPlayerToMove() const {
    return player_to_move;
}

void GameState::SetPlayerToMove(Player player) {
    player_to_move = player;
}

void GameState::SwitchPlayerToMove() {
    SetPlayerToMove(GetPlayerToMove() == Player::Computer ? Player::Human : Player::Computer);
}

void GameState::MakeMove(const Move& move) {
    GetBoard().At(move.row, move.col) = GetPieceToMove();
    SwitchSideToMove();
    UpdateGameStatus();
}

Player GameState::GetPlayerX() const {
    return player_x;
}

void GameState::SetPlayerX(Player player) {
    player_x = player;
}

Player GameState::GetPlayerO() const {
    return player_o;
}

void GameState::SetPlayerO(Player player) {
    player_o = player;
}

ComputerMode GameState::GetComputerMode() const {
    return computer_mode;
}

void GameState::SetComputerMode(ComputerMode mode) {
    computer_mode = mode;
}

AiAlgorithm GameState::GetAiAlgorithm() const {
    return ai_algorithm;
}

void GameState::SetAiAlgorithm(AiAlgorithm algorithm) {
    ai_algorithm = algorithm;
}
