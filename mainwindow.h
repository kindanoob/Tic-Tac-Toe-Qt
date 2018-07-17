#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "board.h"
#include "gamestate.h"
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QActionGroup>

constexpr int kWindowWidthInPx = 640;
constexpr int kWindowHeightInPx = static_cast<int>(kWindowWidthInPx * 3.0 / 4);
constexpr int kWindowMinWidthInPx = 300;
constexpr int kWindowMinHeightInPx = static_cast<int>(kWindowMinWidthInPx * 3.0 / 4);
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

signals:
    //

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
    //Menus
    QMenu *game_menu;
    QMenu *settings_menu;
    QMenu *computer_mode_menu;
    QMenu *ai_algorithm_menu;
    QMenu *window_menu;
    QMenu *help_menu;
    //Actions
    QAction *new_game_action;
    QAction *exit_action;
    QAction *toggle_fullscreen_action;
    QAction *about_action;
    QAction *help_action;
    QAction *computer_plays_x_action;
    QAction *computer_plays_o_action;
    QAction *computer_observes_action;
    QActionGroup *computer_mode_action_group;
    QAction *ai_random_action;
    QAction *ai_minimax_action;
    QActionGroup *ai_action_group;

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void CreateRects();
    void FillSquare(int ind, SideToMove side, QPainter& painter);
    void CreateBoard();
    void CreateActions();
    void CreateMenus();
    //reset stuff

    //update stuff
    void UpdateWindowParameters();
    void UpdateBoardRectParameters();

    void PrintBoardToConsole();
    int GetSquareSizeInPx();

    void MakeComputerMove();

private slots:
    void on_new_game_action_triggered();
    void on_exit_action_triggered();
    void on_toggle_fullscreen_action_triggered();
    void on_about_action_triggered();
    void on_help_action_triggered();
    void on_computer_plays_x_action_triggered();
    void on_computer_plays_o_action_triggered();
    void on_computer_observes_action_triggered();
    void on_ai_random_action_triggered();
    void on_ai_minimax_action_triggered();
};

#endif // MAINWINDOW_H
