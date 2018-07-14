#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QMessageBox>
#include <QApplication>

constexpr int kSquareSizeScaleFactor = 6;
constexpr int kSquareSizeInPx = kWindowWidthInPx / kSquareSizeScaleFactor;
constexpr int kPenWidthInPx = 8;
constexpr int kPenScaleFactor = 10;
constexpr int kNumRows = 3;
constexpr int kNumCols = 3;
constexpr int kNumSquares = kNumRows * kNumCols;
constexpr int kBoardWidthInPx = kSquareSizeInPx * kNumCols;
constexpr int kBoardHeightInPx = kSquareSizeInPx * kNumRows;
constexpr int kXOffsetInPx = (kWindowWidthInPx - kBoardWidthInPx) / 2;
constexpr int kWindowVerticalOffsetInPx = 50;//TODO: How to change it?
constexpr int kYOffsetInPx = (kWindowHeightInPx - kBoardHeightInPx) / 2 + kWindowVerticalOffsetInPx;
constexpr int kCircleRadius = kSquareSizeInPx / 2 - 3 * kPenWidthInPx / 4;
// kCircleCoeff is used to adjust the circle radius depending on the square size and pen's width
constexpr double kCircleCoeff = 5.0 / 5;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    side_to_move(SideToMove::X),
    is_finished(false),
    game_status(GameStatus::InProgress),
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
    CreateBoard();
    CreateRects();
    ui->mainToolBar->setIconSize(QSize(20, 20));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::CreateBoard() {
    for (int row = 0; row < kNumRows; ++row) {
        board.append(QVector<Piece>());
        for (int col = 0; col < kNumCols; ++col) {
            board[row].append(Piece::NoPiece);
        }
    }
}

void MainWindow::ResetBoard() {
    for (auto& row : board) {
        for (auto& square : row) {
            square = Piece::NoPiece;
        }
    }
}

void MainWindow::PrintBoardToConsole() {
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
        if (board[row][col] == Piece::NoPiece) {
            continue;
        } else if (board[row][col] == Piece::X) {
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
        } else if (board[row][col] == Piece::O) {
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
    if (event->button() == Qt::LeftButton) {
        int x = event->pos().x();
        int y = event->pos().y();
        for (int i = 0; i < rects.size(); ++i) {
            int row = i / kNumRows;
            int col = i % kNumCols;
            if (rects[i].contains(QPoint(x, y)) && board[row][col] == Piece::NoPiece) {
                board[row][col] = (side_to_move == SideToMove::X) ? Piece::X : Piece::O;
                UpdateGameStatus();
                if (IsGameFinished()) {
                    QMessageBox msgBox;
                    msgBox.setText(GetGameOutcomeText());
                    msgBox.exec();
                    Reset();
                    return;
                }
                SwitchSideToMove();
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
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col){
            QRect rect(offset_x + col * square_size_in_px,
                       offset_y + row * square_size_in_px,
                       square_size_in_px, square_size_in_px);
            rects.append(rect);
        }
    }
}

void MainWindow::SetSideToMove(const SideToMove& side) {
    side_to_move = side;
}

SideToMove MainWindow::GetSideToMove() {
    return side_to_move;
}

void MainWindow::SwitchSideToMove() {
    if (side_to_move == SideToMove::X) {
        SetSideToMove(SideToMove::O);
    } else {
        SetSideToMove(SideToMove::X);
    }
}

bool MainWindow::IsGameFinished() {
    return game_status != GameStatus::InProgress;
}

void MainWindow::UpdateGameStatus() {
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

bool MainWindow::CheckWin(const SideToMove& side) {
    for (int i = 0; i < kNumRows; ++i) {
        if (CheckRowWin(i, side) || CheckColWin(i, side)) {
            return true;
        }
    }
    return CheckMainDiagWin(side) || CheckAntiDiagWin(side);
}

bool MainWindow::CheckRowWin(int row, const SideToMove& side) {
    Piece piece = side == SideToMove::X ? Piece::X : Piece::O;
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

bool MainWindow::CheckColWin(int col, const SideToMove& side) {
    Piece piece = side == SideToMove::X ? Piece::X : Piece::O;
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

bool MainWindow::CheckMainDiagWin(const SideToMove& side) {
    Piece piece = side == SideToMove::X ? Piece::X : Piece::O;
    for (int row = 0; row < kNumRows; ++row) {
        if (board[row][row] != piece) {
            return false;
        }
    }
    return true;
}

bool MainWindow::CheckAntiDiagWin(const SideToMove& side) {
    Piece piece = side == SideToMove::X ? Piece::X : Piece::O;
    for (int row = 0; row < kNumRows; ++row) {
        if (board[row][kNumRows - row - 1] != piece) {
            return false;
        }
    }
    return true;
}

bool MainWindow::CheckDraw() {
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

QString MainWindow::GetGameOutcomeText() {
    assert(is_finished);
    QString outcome("Game is finished.\n");
    if (game_status == GameStatus::XWon) {
        outcome += "X Won.";
    } else if (game_status == GameStatus::OWon) {
        outcome += "O Won.";
    } else {
        outcome += "Draw.";
    }
    return outcome;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionNew_triggered()
{
    Reset();
}

void MainWindow::Reset() {
    ResetBoard();
    ResetSideToMove();
    ResetGameStatus();
    ResetIsFinished();
    update();
}

void MainWindow::ResetSideToMove() {
    SetSideToMove(SideToMove::X);
}

void MainWindow::ResetGameStatus() {
    SetGameStatus(GameStatus::InProgress);
}

void MainWindow::ResetIsFinished() {
    SetIsFinished(false);
}

void MainWindow::SetIsFinished(bool b) {
    is_finished = b;
}

bool MainWindow::GetIsFinished() {
    return is_finished;
}

void MainWindow::SetGameStatus(const GameStatus &status) {
    game_status = status;
}

GameStatus MainWindow::GetGameStatus() {
    return game_status;
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Tic-Tac-Toe");
    msgBox.setText("Tic-Tac-Toe game\nAuthor: Evgeny Pavlov aka kindanoob\n"
                   "Email: bknd890@gmail.com");
    msgBox.exec();
}

void MainWindow::on_actionToggle_Fullscreen_triggered()
{
    is_fullscreen ? showNormal() : showFullScreen();
    is_fullscreen = !is_fullscreen;
}
