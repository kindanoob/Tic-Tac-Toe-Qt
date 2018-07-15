#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "board.h"
#include <QMainWindow>

constexpr int kWindowWidthInPx = 640;
constexpr int kWindowHeightInPx = static_cast<int>(kWindowWidthInPx * 3.0 / 4);
const QString kWindowTitle = QString("Tic-Tac-Toe");

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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void PrintRectInfo();

private:
    Ui::MainWindow *ui;
    QVector<QRect> rects;
    Board board;
    SideToMove side_to_move;
    bool is_finished;
    GameStatus game_status;
    bool is_fullscreen;
    int window_width;
    int window_height;
    int square_size_in_px;
    int board_width;
    int board_height;
    int circle_radius;
    int offset_x;
    int offset_y;
    int pen_width;

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void CreateRects();
    void FillSquare(int ind, SideToMove side, QPainter& painter);
    void CreateBoard();
    //reset stuff
    void Reset();
    void ResetBoard();
    void ResetSideToMove();
    void ResetGameStatus();
    void ResetIsFinished();

    //update stuff
    void UpdateWindowParameters();
    void UpdateBoardRectParameters();

    void PrintBoardToConsole();
    //setters and getters
    void SetSideToMove(const SideToMove& side);
    SideToMove GetSideToMove();
    void SwitchSideToMove();
    void SetIsFinished(bool b);
    bool GetIsFinished();
    void SetGameStatus(const GameStatus& status);
    GameStatus GetGameStatus();
    void UpdateGameStatus();
    int GetSquareSizeInPx();

    bool IsGameFinished();
    bool CheckWin(const SideToMove& side);
    bool CheckDraw();
    bool CheckRowWin(int row, const SideToMove& side);
    bool CheckColWin(int col, const SideToMove& side);
    bool CheckMainDiagWin(const SideToMove& side);
    bool CheckAntiDiagWin(const SideToMove& side);
    QString GetGameOutcomeText();
private slots:
    void on_actionExit_triggered();
    void on_actionNew_triggered();
    void on_actionAbout_triggered();
    void on_actionToggle_Fullscreen_triggered();
};

#endif // MAINWINDOW_H
