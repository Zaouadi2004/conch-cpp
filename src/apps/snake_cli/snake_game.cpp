#include "snake_game.h"
#include <algorithm>

SnakeGame::SnakeGame(int width, int height) 
    : width_(width), height_(height), score_(0), game_over_(false),
      rng_(std::random_device{}()) {
    reset();
}

void SnakeGame::reset() {
    snake_.clear();
    snake_.push_back({width_ / 2, height_ / 2});
    direction_ = Direction::RIGHT;
    score_ = 0;
    game_over_ = false;
    generateFood();
}

bool SnakeGame::update() {
    if (game_over_) return false;
    
    Point head = snake_.front();
    
    // Move head based on direction
    switch (direction_) {
        case Direction::UP:    head.y--; break;
        case Direction::DOWN:  head.y++; break;
        case Direction::LEFT:  head.x--; break;
        case Direction::RIGHT: head.x++; break;
    }
    
    // Check collision
    if (checkCollision(head)) {
        game_over_ = true;
        return false;
    }
    
    snake_.push_front(head);
    
    // Check if ate food
    if (head == food_) {
        score_++;
        generateFood();
    } else {
        snake_.pop_back();
    }
    
    return true;
}

void SnakeGame::changeDirection(Direction dir) {
    // Prevent 180-degree turns
    if ((direction_ == Direction::UP && dir == Direction::DOWN) ||
        (direction_ == Direction::DOWN && dir == Direction::UP) ||
        (direction_ == Direction::LEFT && dir == Direction::RIGHT) ||
        (direction_ == Direction::RIGHT && dir == Direction::LEFT)) {
        return;
    }
    direction_ = dir;
}

void SnakeGame::generateFood() {
    std::uniform_int_distribution<> dist_x(0, width_ - 1);
    std::uniform_int_distribution<> dist_y(0, height_ - 1);
    
    do {
        food_ = {dist_x(rng_), dist_y(rng_)};
    } while (std::find(snake_.begin(), snake_.end(), food_) != snake_.end());
}

bool SnakeGame::checkCollision(const Point& head) {
    // Wall collision
    if (head.x < 0 || head.x >= width_ || head.y < 0 || head.y >= height_) {
        return true;
    }
    
    // Self collision
    return std::find(snake_.begin(), snake_.end(), head) != snake_.end();
}
