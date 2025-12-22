#include "game_widget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>
#include <algorithm>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent), score_(0), game_over_(false), paused_(false),
      rng_(std::random_device{}()) {
    
    setFixedSize(GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE + 50);
    setFocusPolicy(Qt::StrongFocus);
    
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &GameWidget::gameLoop);
    
    reset();
    timer_->start(150);
}

void GameWidget::reset() {
    snake_.clear();
    snake_.push_back({GRID_SIZE / 2, GRID_SIZE / 2});
    direction_ = Direction::RIGHT;
    score_ = 0;
    game_over_ = false;
    paused_ = false;
    generateFood();
    update();
}

void GameWidget::generateFood() {
    std::uniform_int_distribution<> dist(0, GRID_SIZE - 1);
    
    do {
        food_ = {dist(rng_), dist(rng_)};
    } while (std::find(snake_.begin(), snake_.end(), food_) != snake_.end());
}

bool GameWidget::checkCollision(const Point& head) {
    if (head.x < 0 || head.x >= GRID_SIZE || head.y < 0 || head.y >= GRID_SIZE) {
        return true;
    }
    return std::find(snake_.begin(), snake_.end(), head) != snake_.end();
}

void GameWidget::gameLoop() {
    if (game_over_ || paused_) return;
    
    Point head = snake_.front();
    
    switch (direction_) {
        case Direction::UP:    head.y--; break;
        case Direction::DOWN:  head.y++; break;
        case Direction::LEFT:  head.x--; break;
        case Direction::RIGHT: head.x++; break;
    }
    
    if (checkCollision(head)) {
        game_over_ = true;
        timer_->stop();
        QMessageBox::information(this, "Game Over", 
            QString("Game Over! Score: %1\nPress R to restart").arg(score_));
        return;
    }
    
    snake_.push_front(head);
    
    if (head == food_) {
        score_++;
        generateFood();
    } else {
        snake_.pop_back();
    }
    
    update();
}

void GameWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    
    // Draw background
    painter.fillRect(0, 0, width(), height(), Qt::black);
    
    // Draw grid
    painter.setPen(QPen(Qt::darkGray, 1));
    for (int i = 0; i <= GRID_SIZE; ++i) {
        painter.drawLine(i * CELL_SIZE, 0, i * CELL_SIZE, GRID_SIZE * CELL_SIZE);
        painter.drawLine(0, i * CELL_SIZE, GRID_SIZE * CELL_SIZE, i * CELL_SIZE);
    }
    
    // Draw snake
    for (size_t i = 0; i < snake_.size(); ++i) {
        QColor color = (i == 0) ? Qt::green : Qt::darkGreen;
        painter.fillRect(snake_[i].x * CELL_SIZE + 2, 
                        snake_[i].y * CELL_SIZE + 2,
                        CELL_SIZE - 4, CELL_SIZE - 4, color);
    }
    
    // Draw food
    painter.fillRect(food_.x * CELL_SIZE + 2, 
                    food_.y * CELL_SIZE + 2,
                    CELL_SIZE - 4, CELL_SIZE - 4, Qt::red);
    
    // Draw score
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 14));
    painter.drawText(10, GRID_SIZE * CELL_SIZE + 30, 
                    QString("Score: %1 | Space: Pause | R: Restart").arg(score_));
    
    if (paused_) {
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "PAUSED");
    }
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Up:
        case Qt::Key_W:
            if (direction_ != Direction::DOWN)
                direction_ = Direction::UP;
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            if (direction_ != Direction::UP)
                direction_ = Direction::DOWN;
            break;
        case Qt::Key_Left:
        case Qt::Key_A:
            if (direction_ != Direction::RIGHT)
                direction_ = Direction::LEFT;
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            if (direction_ != Direction::LEFT)
                direction_ = Direction::RIGHT;
            break;
        case Qt::Key_Space:
            paused_ = !paused_;
            update();
            break;
        case Qt::Key_R:
            reset();
            timer_->start(150);
            break;
    }
}
