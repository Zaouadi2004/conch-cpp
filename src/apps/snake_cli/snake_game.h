#pragma once
#include <vector>
#include <deque>
#include <random>

enum class Direction { UP, DOWN, LEFT, RIGHT };

struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class SnakeGame {
public:
    SnakeGame(int width = 20, int height = 20);
    
    void reset();
    bool update();  // Returns false if game over
    void changeDirection(Direction dir);
    
    const std::deque<Point>& getSnake() const { return snake_; }
    const Point& getFood() const { return food_; }
    int getScore() const { return score_; }
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    bool isGameOver() const { return game_over_; }
    
private:
    void generateFood();
    bool checkCollision(const Point& head);
    
    int width_, height_;
    std::deque<Point> snake_;
    Point food_;
    Direction direction_;
    int score_;
    bool game_over_;
    std::mt19937 rng_;
};
