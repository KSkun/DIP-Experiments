#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString fileImage, fileEncoded, fileCode;
    QImage *image;

    void refreshImageView();

    void refreshFileView();

private slots:
    void on_buttonQuit_clicked();

    void on_buttonFileImage_clicked();

    void on_buttonFileEncoded_clicked();

    void on_buttonFileCode_clicked();
};

#endif // MAINWINDOW_H
