#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include <QFileDialog>
#include <QPixmap>
#include <QImageReader>
#include <QSignalBlocker>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectsInit();
    namesInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectsInit(){
    connect(ui->pushButtonWskazanieSciezki, &QPushButton::clicked, this, &MainWindow::browseFile);
    connect(ui->spinBoxNewResWidth, &QSpinBox::valueChanged, this, &MainWindow::NewImageWidthChangedAction);
    connect(ui->spinBoxNewResHeight, &QSpinBox::valueChanged, this, &MainWindow::NewImageHeightChangedAction);
    connect(ui->pushButtonPrzeskaluj, &QPushButton::clicked, this, &MainWindow::generateNewImage);
    connect(ui->pushButtonZapisz, &QPushButton::clicked, this, &MainWindow::saveNewImage);
}

void MainWindow::namesInit(){
    ui->pushButtonGenerujRGB565->setText("Generate RGB565");
    ui->pushButtonPrzeskaluj->setText("Scale");
    ui->pushButtonWskazanieSciezki->setText("Browse File");
    ui->pushButtonZapisz->setText("Save");

    ui->labelNaglowek->setText("RGB 565 Image Converter");
    ui->labelPodgladObrazka->setText("");
    ui->labelPodgladPoPrzeskalowaniu->setText("");
    ui->labelWskazanaSciezka->setText("");

    ui->groupBoxAktualneParametry->setTitle("Current Parameters");
    ui->groupBoxNoweParametry->setTitle("New Parameters");
    ui->labelCurrResName->setText("Resolution");
    ui->labelCurrResVal->setText("0x0");
    ui->labelCurrFormatName->setText("Format");
    ui->labelCurrFormatVal->setText("");
    ui->labelNewResName->setText("Resolution");
    ui->labelNewResX->setText("x");
    ui->spinBoxNewResWidth->setValue(0);
    ui->spinBoxNewResHeight->setValue(0);
    ui->labelNewFormatName->setText("Format");
    ui->labelNewFormatVal->setText("");

}

void MainWindow::browseFile(){
    filePath = QFileDialog::getOpenFileName(
        this,
        "Choose Picture",
        "",
        "Images (*.png *.jpg *.jpeg *.bmp)"
        );
    if(!filePath.isNull()){
        ui->labelWskazanaSciezka->setText(filePath);
        showImage();
    }
}

void MainWindow::showImage(){
    QImage tmpImage(filePath);
    if(!tmpImage.isNull()){
        CurrImage = tmpImage;
        QPixmap pixmap = QPixmap::fromImage(tmpImage);
        ui->labelPodgladObrazka->setPixmap(
            pixmap.scaled(
                ui->labelPodgladObrazka->size(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            )
        );
        refreshCurrentImageInfo();
    }
}

void MainWindow::refreshCurrentImageInfo(){
    QString Width = QString::number(CurrImage.width());
    QString Height = QString::number(CurrImage.height());
    QString resVal = Width + "x" + Height;

    QImageReader reader(filePath);
    QString format = reader.format().toUpper();

    ui->labelCurrResVal->setText(resVal);
    ui->spinBoxNewResWidth->setValue(CurrImage.width());
    ui->spinBoxNewResHeight->setValue(CurrImage.height());
    ui->labelCurrFormatVal->setText(format);
    ui->labelNewFormatVal->setText(format);
}

void MainWindow::NewImageWidthChangedAction(){
    NewImageWidth = ui->spinBoxNewResWidth->value();
    QSignalBlocker blocker(ui->spinBoxNewResHeight);
    double ratio = (double)CurrImage.height() / CurrImage.width();
    NewImageHeight = ratio * NewImageWidth;
    ui->spinBoxNewResHeight->setValue(NewImageHeight);
}

void MainWindow::NewImageHeightChangedAction(){
    NewImageHeight = ui->spinBoxNewResHeight->value();
    QSignalBlocker blocker(ui->spinBoxNewResWidth);
    double ratio = (double)CurrImage.width() / CurrImage.height();
    NewImageWidth = ratio * NewImageHeight;
    ui->spinBoxNewResWidth->setValue(NewImageWidth);
}

void MainWindow::generateNewImage(){
    NewImage = CurrImage.scaled(
        NewImageWidth,
        NewImageHeight,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );

    ui->labelPodgladPoPrzeskalowaniu->setPixmap(QPixmap::fromImage(NewImage));
}

void MainWindow::saveNewImage(){
    QString saveFilePath = QFileDialog::getSaveFileName(
        this,
        "Save Image",
        "",
        "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)"
    );
    if(!saveFilePath.isEmpty()){
        if(!NewImage.save(saveFilePath)){
            qDebug() << "Nie udalo sie :<";
        }
    }
}