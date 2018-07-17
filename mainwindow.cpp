#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ai.h"
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QMessageBox>
#include <QApplication>
#include <QPixmap>
#include <QIcon>

constexpr int kSquareSizeScaleFactor = 6;
constexpr int kSquareSizeInPx = kWindowWidthInPx / kSquareSizeScaleFactor;
constexpr int kPenWidthInPx = 8;
constexpr int kPenScaleFactor = 10;

constexpr int kBoardWidthInPx = kSquareSizeInPx * kNumCols;
constexpr int kBoardHeightInPx = kSquareSizeInPx * kNumRows;
constexpr int kXOffsetInPx = (kWindowWidthInPx - kBoardWidthInPx) / 2;
constexpr int kWindowVerticalOffsetInPx = 50;//TODO: How to change it?
constexpr int kYOffsetInPx = (kWindowHeightInPx - kBoardHeightInPx) / 2 + kWindowVerticalOffsetInPx;
constexpr int kCircleRadius = kSquareSizeInPx / 2 - 3 * kPenWidthInPx / 4;
// kCircleCoeff is used to adjust the circle radius depending on the square size and pen's width
constexpr double kCircleCoeff = 5.0 / 5;

constexpr int kMenuIconWidthInPx = 30;
constexpr int kMenuIconHeightInPx = 30;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    is_fullscreen(false),
    window_width(kWindowWidthInPx),
    window_height(kWindowHeightInPx),
    square_size_in_px(kSquareSizeInPx),
    board_width(kBoardWidthInPx),
    board_height(kBoardHeightInPx),
    circle_radius(kCircleRadius),
    offset_x(kXOffsetInPx),
    offset_y(kYOffsetInPx),
    pen_width(kPenWidthInPx)
{
    ui->setupUi(this);
    setMouseTracking(true);
    CreateRects();
    ui->mainToolBar->setIconSize(QSize(kMenuIconWidthInPx, kMenuIconHeightInPx));
    setMinimumSize(kWindowMinWidthInPx, kWindowMinHeightInPx);
    resize(kWindowWidthInPx, kWindowHeightInPx);
    setWindowTitle(kWindowTitle);
    CreateActions();
    CreateMenus();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::CreateActions() {
    new_game_action = new QAction(tr("&New Game"), this);
    new_game_action->setShortcut(tr("Ctrl+N"));
    new_game_action->setStatusTip(tr("Start new game"));
    new_game_action->setIcon(QIcon(":/images/menu_icons/play.jpg"));
    connect(new_game_action, SIGNAL(triggered()), this, SLOT(on_new_game_action_triggered()));
    //connect(this, &Menu::, this, &MainWindow::on_new_game_act_triggered);
    //computer_mode_alignment_group = new QActionGroup(this);
    //computer_mode_alignment_group->addAction

    exit_action = new QAction(tr("&Exit"), this);
    exit_action->setShortcut(tr("Ctrl+Q"));
    exit_action->setStatusTip(tr("Exit"));
    exit_action->setIcon(QIcon(":/images/menu_icons/exit.jpg"));
    exit_action->setToolTip("LOOOOOOOOOOL");
    connect(exit_action, SIGNAL(triggered()), this, SLOT(on_exit_action_triggered()));

    toggle_fullscreen_action = new QAction(tr("&Toggle fullscreen"), this);
    toggle_fullscreen_action->setShortcut(tr("Ctrl+F"));
    toggle_fullscreen_action->setStatusTip(tr("Toggle fullscreen"));
    toggle_fullscreen_action->setIcon(QIcon(":/images/menu_icons/fullscreen.jpg"));
    connect(toggle_fullscreen_action, SIGNAL(triggered()), this, SLOT(on_toggle_fullscreen_action_triggered()));

    about_action = new QAction(tr("&About"), this);
    about_action->setShortcut(tr("&Ctrl+A"));
    about_action->setStatusTip(tr("About"));
    about_action->setIcon(QIcon(":/images/menu_icons/about.jpg"));
    connect(about_action, SIGNAL(triggered()), this, SLOT(on_about_action_triggered()));

    computer_plays_x_action = new QAction(tr("&Computer plays X"), this);
    connect(computer_plays_x_action, SIGNAL(triggered()), this, SLOT(on_computer_plays_x_action_triggered()));

    computer_plays_o_action = new QAction(tr("&Computer plays O"), this);
    connect(computer_plays_o_action, SIGNAL(triggered()), this, SLOT(on_computer_plays_o_action_triggered()));

    computer_observes_action = new QAction(tr("&Computer observes"), this);
    connect(computer_observes_action, SIGNAL(triggered()), this, SLOT(on_computer_observes_action_triggered()));

    computer_mode_action_group = new QActionGroup(this);
    computer_mode_action_group->addAction(computer_plays_x_action);
    computer_mode_action_group->addAction(computer_plays_o_action);
    computer_mode_action_group->addAction(computer_observes_action);
    computer_plays_x_action->setCheckable(true);
    computer_plays_o_action->setCheckable(true);
    computer_observes_action->setCheckable(true);
    computer_plays_o_action->setChecked(true);

    help_action = new QAction(tr("&Help"), this);
    help_action->setShortcut(tr("Ctrl+H"));
    help_action->setStatusTip(tr("Help"));
    help_action->setIcon(QIcon(":/images/menu_icons/help.jpg"));
    connect(help_action, SIGNAL(triggered()), this, SLOT(on_help_action_triggered()));
}

void MainWindow::CreateMenus() {
    game_menu = menuBar()->addMenu(tr("&Game"));
    game_menu->addAction(new_game_action);
    game_menu->addSeparator();
    game_menu->addAction(exit_action);

    settings_menu = menuBar()->addMenu(tr("&Settings"));
    settings_menu->addAction(computer_plays_x_action);
    settings_menu->addAction(computer_plays_o_action);
    settings_menu->addAction(computer_observes_action);

    window_menu = menuBar()->addMenu(tr("Window"));
    window_menu->addAction(toggle_fullscreen_action);

    help_menu = menuBar()->addMenu(tr("&Help"));
    help_menu->addAction(about_action);
    help_menu->addAction(help_action);
}

void MainWindow::UpdateBoardRectParameters() {
    rects.clear();
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col){
            QRect rect(offset_x + col * square_size_in_px, offset_y + row * square_size_in_px,
                       square_size_in_px, square_size_in_px);
            rects.append(rect);
        }
    }
}

void MainWindow::UpdateWindowParameters() {
    window_width = centralWidget()->geometry().width();
    window_height = centralWidget()->geometry().height();
    square_size_in_px = qMin(window_width, window_height) / kSquareSizeScaleFactor;
    board_width = square_size_in_px * kNumCols;
    board_height = square_size_in_px * kNumRows;
    circle_radius = square_size_in_px / 2 - kCircleCoeff * kPenWidthInPx;
    offset_x = (window_width - board_width) / 2;
    offset_y = (window_height - board_height) / 2 + kWindowVerticalOffsetInPx;
    pen_width = square_size_in_px / kPenScaleFactor;
    UpdateBoardRectParameters();
}

int MainWindow::GetSquareSizeInPx() {
    return square_size_in_px;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    UpdateWindowParameters();
    QPainter painter(this);
    for (const auto& rect : rects) {
        painter.drawPolygon(rect);
    }

    QPen pen;
    pen.setWidth(pen_width);
    for (int i = 0; i < kNumSquares; ++i) {
        int row = i / kNumRows;
        int col = i % kNumCols;
        int ox = offset_x;
        int oy = offset_y;
        int sqsz = square_size_in_px;
        int r = circle_radius;
        if (GetGameState().GetBoard().At(row, col) == Piece::NoPiece) {
            continue;
        } else if (GetGameState().GetBoard().At(row, col) == Piece::X) {
            pen.setColor(Qt::red);
            pen.setCapStyle(Qt::RoundCap);
            painter.setPen(pen);
            painter.drawLine(ox + col * sqsz + pen_width,
                             oy + row * sqsz + pen_width,
                             ox + col * sqsz + sqsz - pen_width,
                             oy + row * sqsz + sqsz - pen_width);
            painter.drawLine(ox + col * sqsz + sqsz - pen_width,
                             oy + row * sqsz + pen_width,
                             ox + col * sqsz + pen_width,
                             oy + row * sqsz + sqsz - pen_width);
        } else if (GetGameState().GetBoard().At(row, col) == Piece::O) {
            pen.setColor(Qt::green);
            painter.setPen(pen);
            painter.drawEllipse(QPoint(ox + col * sqsz + sqsz / 2, oy + row * sqsz + sqsz / 2),
                                r, r);
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    update();
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (GetGameState().GetPlayerToMove() == Player::Computer) {
        return;
    }
    if (event->button() == Qt::LeftButton) {
        int x = event->pos().x();
        int y = event->pos().y();
        for (int i = 0; i < rects.size(); ++i) {
            int row = i / kNumRows;
            int col = i % kNumCols;
            Move player_move(row, col);
            if (rects[i].contains(QPoint(x, y)) && GetGameState().GetBoard().At(row, col) == Piece::NoPiece) {
                GetGameState().MakeMove(player_move);
                if (GetGameState().IsGameFinished()) {
                    QMessageBox msgBox;
                    msgBox.setText(GetGameState().GetGameOutcomeText());
                    msgBox.exec();
                    GetGameState().Reset();
                    if (GetGameState().GetComputerMode() == ComputerMode::kPlaysX) {
                        MakeComputerMove();
                    }
                    return;
                }
                if (GetGameState().GetComputerMode() != ComputerMode::kObserves) {
                    GetGameState().SwitchPlayerToMove();
                    MakeComputerMove();
                }
                break;
            }
        }
    }
    update();
}

void MainWindow::PrintRectInfo() {
    qDebug() << "In PrintRectInfo";
    for (const auto& rect : rects) {
        qDebug() << "rect: " << rect;
    }
    qDebug() << "Out PrintRectInfo";
}

void MainWindow::CreateRects() {
    for (int row = 0; row < kNumRows; ++row) {
        for (int col = 0; col < kNumCols; ++col){
            QRect rect(offset_x + col * square_size_in_px,
                       offset_y + row * square_size_in_px,
                       square_size_in_px, square_size_in_px);
            rects.append(rect);
        }
    }
}

void MainWindow::on_exit_action_triggered() {
    QApplication::exit();
}

void MainWindow::on_new_game_action_triggered() {
    GetGameState().Reset();
    update();
    if (GetGameState().GetPlayerToMove() == Player::Computer) {
        MakeComputerMove();
    }
}

void MainWindow::on_about_action_triggered() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Tic-Tac-Toe");
    msgBox.setText("Tic-Tac-Toe game\nAuthor: Evgeny Pavlov aka kindanoob\n"
                   "Email: bknd890@gmail.com");
    msgBox.exec();
}

void MainWindow::on_toggle_fullscreen_action_triggered() {
    is_fullscreen ? showNormal() : showFullScreen();
    is_fullscreen = !is_fullscreen;
}

void MainWindow::on_help_action_triggered() {
    //
}

void MainWindow::on_computer_plays_x_action_triggered() {
    GetGameState().SetComputerMode(ComputerMode::kPlaysX);
    GetGameState().SetPlayerX(Player::Computer);
    if (GetGameState().GetSideToMove() == SideToMove::X) {
        GetGameState().SetPlayerToMove(Player::Computer);
        qDebug() << "Set player to move to computer";
        MakeComputerMove();
    }
}

void MainWindow::on_computer_plays_o_action_triggered() {
    GetGameState().SetComputerMode(ComputerMode::kPlaysO);
    GetGameState().SetPlayerO(Player::Computer);
    if (GetGameState().GetSideToMove() == SideToMove::O) {
        GetGameState().SetPlayerToMove(Player::Computer);
        MakeComputerMove();
    }
}

void MainWindow::on_computer_observes_action_triggered() {
    GetGameState().SetComputerMode(ComputerMode::kObserves);
    GetGameState().SetPlayerX(Player::Human);
    GetGameState().SetPlayerO(Player::Human);
    GetGameState().SetPlayerToMove(Player::Human);
}

GameState& MainWindow::GetGameState() {
    return game_state;
}

const GameState& MainWindow::GetGameState() const {
    return game_state;
}

void MainWindow::MakeComputerMove() {
    //assert(false);
    assert(GetGameState().GetPlayerToMove() == Player::Computer);
    Move computer_move = ai::GetRandomeMove(GetGameState().GetBoard(),
                                            GetGameState().GetSideToMove());
    GetGameState().MakeMove(computer_move);
    if (GetGameState().IsGameFinished()) {
        QMessageBox msgBox;
        msgBox.setText(GetGameState().GetGameOutcomeText());
        msgBox.exec();
        GetGameState().Reset();
        if (GetGameState().GetComputerMode() == ComputerMode::kPlaysX ||
                GetGameState().GetComputerMode() == ComputerMode::kPlaysBoth) {
            MakeComputerMove();
        }
    } else {
        GetGameState().SwitchPlayerToMove();
    }
    update();
}
