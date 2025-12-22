#include <QApplication>
#include <QMainWindow>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    spdlog::info("Desktop Pro application starting...");
    
    QMainWindow window;
    window.setWindowTitle("Conch Cross-Platform Desktop");
    window.resize(800, 600);
    window.show();
    
    return app.exec();
}
