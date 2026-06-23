#include <QApplication>
#include "ui/mainwindow.h"
#include <iostream>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    app.setApplicationName("Qcord");
    app.setApplicationVersion("1.0.0");
    
    std::cout << "Qcord - Streaming & Recording Software v1.0.0" << std::endl;
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
