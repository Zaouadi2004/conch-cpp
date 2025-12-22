#include "snake_game.h"
#include <spdlog/spdlog.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// Terminal control
void setCursorPosition(int x, int y) {
    std::cout << "\033[" << y << ";" << x << "H";
}

void clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void hideCursor() {
    std::cout << "\033[?25l" << std::flush;
}

void showCursor() {
    std::cout << "\033[?25h" << std::flush;
}

// Non-blocking keyboard input
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

void render(const SnakeGame& game) {
    clearScreen();
    
    // Draw top border
    std::cout << "+";
    for (int i = 0; i < game.getWidth(); ++i) std::cout << "-";
    std::cout << "+\n";
    
    // Draw game area
    for (int y = 0; y < game.getHeight(); ++y) {
        std::cout << "|";
        for (int x = 0; x < game.getWidth(); ++x) {
            Point p{x, y};
            bool is_snake = false;
            bool is_head = false;
            
            for (size_t i = 0; i < game.getSnake().size(); ++i) {
                if (game.getSnake()[i] == p) {
                    is_snake = true;
                    is_head = (i == 0);
                    break;
                }
            }
            
            if (is_head) {
                std::cout << "O";
            } else if (is_snake) {
                std::cout << "o";
            } else if (game.getFood() == p) {
                std::cout << "*";
            } else {
                std::cout << " ";
            }
        }
        std::cout << "|\n";
    }
    
    // Draw bottom border
    std::cout << "+";
    for (int i = 0; i < game.getWidth(); ++i) std::cout << "-";
    std::cout << "+\n";
    
    std::cout << "Score: " << game.getScore() << "\n";
    std::cout << "Controls: WASD to move, Q to quit\n";
    std::cout << std::flush;
}

int main() {
    spdlog::info("Starting CLI Snake Game...");
    
    SnakeGame game(20, 15);
    hideCursor();
    
    bool running = true;
    auto last_update = std::chrono::steady_clock::now();
    const auto update_interval = std::chrono::milliseconds(150);
    
    while (running && !game.isGameOver()) {
        // Handle input
        if (kbhit()) {
            char c = getchar();
            switch (c) {
                case 'w': case 'W': game.changeDirection(Direction::UP); break;
                case 's': case 'S': game.changeDirection(Direction::DOWN); break;
                case 'a': case 'A': game.changeDirection(Direction::LEFT); break;
                case 'd': case 'D': game.changeDirection(Direction::RIGHT); break;
                case 'q': case 'Q': running = false; break;
            }
        }
        
        // Update game
        auto now = std::chrono::steady_clock::now();
        if (now - last_update >= update_interval) {
            game.update();
            render(game);
            last_update = now;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    showCursor();
    
    if (game.isGameOver()) {
        std::cout << "\nGame Over! Final Score: " << game.getScore() << "\n";
    }
    
    spdlog::info("CLI Snake Game ended. Score: {}", game.getScore());
    return 0;
}
