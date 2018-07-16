#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "board.h"
#include "gamestate.h"
#include <QMainWindow>

constexpr int kWindowWidthInPx = 640;
constexpr int kWindowHeightInPx = static_cast<int>(kWindowWidthInPx * 3.0 / 4);
const QString kWindowTitle = QString("Tic-Tac-Toe");

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
    GameState& GetGameState();
    const GameState& GetGameState() const;

private:
    Ui::MainWindow *ui;
    GameState game_state;
    QVector<QRect> rects;
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

    //update stuff
    void UpdateWindowParameters();
    void UpdateBoardRectParameters();

    void PrintBoardToConsole();
    int GetSquareSizeInPx();

private slots:
    void on_actionExit_triggered();
    void on_actionNew_triggered();
    void on_actionAbout_triggered();
    void on_actionToggle_Fullscreen_triggered();
};

#endif // MAINWINDOW_H
