#include <QFileDialog>
#include <QDir>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_buttonQuit_clicked() {
    exit(0);
}

void MainWindow::on_buttonFileImage_clicked() {
    auto file = QDir::toNativeSeparators(QFileDialog::getOpenFileName(
            this,
            tr("打开要编码的文件"),
            QDir::currentPath(),
            tr("图片 (*.png *.jpg *.jpeg *.bmp)")
    ));
    if (file.isEmpty()) return;

    fileImage = file;
    fileEncoded = fileCode = "";

    auto newImage = new QImage(fileImage);
    if (newImage->isNull()) {
        ui->statusBar->showMessage(tr("打开文件失败"));
        return;
    }
    if (!newImage->isGrayscale()) {
        ui->statusBar->showMessage(tr("图片不是灰阶图"));
        return;
    }
    delete image;
    image = newImage;

    refreshFileView();
    refreshImageView();
    refreshInfoView();
}

void MainWindow::on_buttonFileEncoded_clicked() {
    auto file = QDir::toNativeSeparators(QFileDialog::getOpenFileName(
            this,
            tr("打开要解码的文件"),
            QDir::currentPath(),
            tr("已编码文件 (*.bin)")
    ));
    if (file.isEmpty()) return;

    fileEncoded = file;
    fileImage = "";

    refreshFileView();
}

void MainWindow::on_buttonFileCode_clicked() {
    auto file = QDir::toNativeSeparators(QFileDialog::getOpenFileName(
            this,
            tr("打开解码使用的编码表"),
            QDir::currentPath(),
            tr("编码表 (*.code.bin)")
    ));
    if (file.isEmpty()) return;

    fileCode = file;
    fileImage = "";

    refreshFileView();
}

void MainWindow::refreshImageView() {
    ui->statusBar->showMessage(tr(""));
    ui->labelImageView->setPixmap(
            QPixmap::fromImage(*image).scaled(ui->labelImageView->size(),
                                             Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation)
    );
}

void MainWindow::refreshFileView() {
    QFontMetrics fontMatrics(ui->labelFileImage->font());
    auto labelWidth = ui->labelFileImage->size().width();

    ui->labelFileImage->setText(fontMatrics.elidedText(fileImage, Qt::ElideMiddle, labelWidth));
    ui->labelFileEncoded->setText(fontMatrics.elidedText(fileEncoded, Qt::ElideMiddle, labelWidth));
    ui->labelFileCode->setText(fontMatrics.elidedText(fileCode, Qt::ElideMiddle, labelWidth));
}

void MainWindow::refreshInfoView() {
    ui->labelSize->setText(QString::asprintf("%d x %d 像素", image->width(), image->height()));
}
