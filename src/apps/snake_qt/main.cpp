#include "game_widget.h"
#include <QApplication>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
    spdlog::info("Starting Qt Snake Game...");
    
    QApplication app(argc, argv);
    
    GameWidget game;
    game.setWindowTitle("Snake Game - Qt6");
    game.show();
    
    return app.exec();
}
