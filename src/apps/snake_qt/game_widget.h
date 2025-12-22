#pragma once
#include <QWidget>
#include <QTimer>
#include <deque>
#include <random>

enum class Direction { UP, DOWN, LEFT, RIGHT };

struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class GameWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit GameWidget(QWidget *parent = nullptr);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    
private slots:
    void gameLoop();
    
private:
    void reset();
    void generateFood();
    bool checkCollision(const Point& head);
    
    static constexpr int GRID_SIZE = 20;
    static constexpr int CELL_SIZE = 25;
    
    std::deque<Point> snake_;
    Point food_;
    Direction direction_;
    int score_;
    bool game_over_;
    bool paused_;
    std::mt19937 rng_;
    QTimer *timer_;
};
