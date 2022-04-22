#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icon.png"));
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
