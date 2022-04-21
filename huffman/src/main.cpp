#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QIcon icon(":/icon.png");
    app.setWindowIcon(icon);
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
