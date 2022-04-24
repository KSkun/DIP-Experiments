#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "huffman.h"

namespace Ui {
    class MainWindow;
}

#define IMAGE_INFO_UNAVAILABLE (-1)

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString fileImage, fileEncoded, fileCode;
    QImage *image = nullptr;
    double imageEntropy = IMAGE_INFO_UNAVAILABLE;
    qint8 imageBinLen = IMAGE_INFO_UNAVAILABLE;
    double imageLenAvg = IMAGE_INFO_UNAVAILABLE;
    HuffmanEncoder *encoder = nullptr;

    void refreshImageView();

    void refreshFileView();

    void refreshInfoView();

private slots:
    void on_buttonQuit_clicked();

    void on_buttonFileImage_clicked();

    void on_buttonFileEncoded_clicked();

    void on_buttonFileCode_clicked();

    void on_buttonEncode_clicked();

    void on_buttonDecode_clicked();
};

#endif // MAINWINDOW_H
