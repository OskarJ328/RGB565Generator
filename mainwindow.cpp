#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include <QFileDialog>
#include <QPixmap>
#include <QImageReader>
#include <QSignalBlocker>
#include <QMessageBox>
#include <QFontDatabase>
#include <QPainter>
#include <QFontMetrics>


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
    connect(ui->pushButtonWskazanieSciezki, &QPushButton::clicked, this, &MainWindow::browseImageFile);
    connect(ui->spinBoxNewResWidth, &QSpinBox::valueChanged, this, &MainWindow::NewImageWidthChangedAction);
    connect(ui->spinBoxNewResHeight, &QSpinBox::valueChanged, this, &MainWindow::NewImageHeightChangedAction);
    connect(ui->pushButtonPrzeskaluj, &QPushButton::clicked, this, &MainWindow::generateNewImage);
    connect(ui->pushButtonZapisz, &QPushButton::clicked, this, &MainWindow::saveNewImage);
    connect(ui->pushButtonGenerujRGB565, &QPushButton::clicked, this, &MainWindow::generateRGB565Array);

    connect(ui->pushButton_BrowseFont, &QPushButton::clicked, this, &MainWindow::browseFontFile);
    connect(ui->spinBox_fontSize, &QSpinBox::valueChanged, this, &MainWindow::fontSizeChangedAction);
}

void MainWindow::namesInit(){

    ui->tabWidget->setTabText(0, "Images");
    ui->tabWidget->setTabText(1, "Fonts");

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
    ui->labelEnterArrayName->setText("Enter Array Name");



    ui->label_fontPath->setText("");
    ui->label_enterFontNameTitle->setText("Enter font name");
    ui->label_fontPreviewTitle->setText("Font preview");
    ui->label_fontPreviewImage->setText("");
    ui->pushButton_BrowseFont->setText("Browse file");
    ui->pushButton_generateFont->setText("Generate font");

    ui->label_fontHeightTitle->setText("Height");
    ui->label_fontWidthTitle->setText("Width");
    ui->label_fontHeightVal->setText("0");
    ui->label_fontWidthVal->setText("0");
    ui->label_fontSize->setText("font Size");


}

void MainWindow::browseImageFile(){
    imageFilePath = QFileDialog::getOpenFileName(
        this,
        "Choose Picture",
        "",
        "Images (*.png *.jpg *.jpeg *.bmp)"
        );
    if(!imageFilePath.isNull()){
        ui->labelWskazanaSciezka->setText(imageFilePath);
        showImage();
    }
}

void MainWindow::showImage(){
    QImage tmpImage(imageFilePath);
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

    QImageReader reader(imageFilePath);
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

    QPixmap pixmap = QPixmap::fromImage(NewImage);
    ui->labelPodgladPoPrzeskalowaniu->setPixmap(
        pixmap.scaled(
            ui->labelPodgladPoPrzeskalowaniu->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            )
        );

}

void MainWindow::saveNewImage(){
    QString saveimageFilePath = QFileDialog::getSaveFileName(
        this,
        "Save Image",
        "",
        "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)"
    );
    if(!saveimageFilePath.isEmpty()){
        if(!NewImage.save(saveimageFilePath)){
            qDebug() << "Nie udalo sie :<";
        }
    }
}

void MainWindow::convertToRGB565(){
    for(int y = 0; y < NewImage.height(); y++){
        for(int x = 0; x < NewImage.width(); x++){
            QColor color = NewImage.pixelColor(x, y);
            int r = color.red();
            int g = color.green();
            int b = color.blue();
            uint16_t rgb565_pixel = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
            RGB565.append(rgb565_pixel);
        }
    }
}

void MainWindow::generateRGB565Array(){
    QString folder = QFileDialog::getExistingDirectory(this, "Choose Directory to save");
    if(!folder.isEmpty()){
        convertToRGB565();
        QString fileName = ui->textEditArrayNameVal->toPlainText();
        QString imageFilePath = folder + "/" + fileName + ".c";
        QString structName = fileName + QString::number(NewImage.width()) + "x" + QString::number(NewImage.height());
        QString arrayName = structName + "_buffer";
        qDebug() <<imageFilePath;
        QFile file(imageFilePath);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QMessageBox::warning(this, "Error", "Failed at opening file");
            return;
        }
        QTextStream out(&file);
        out << "static const uint16_t " << arrayName << "[] =\n{\n";
        for(int i = 0; i < RGB565.size(); i++){
            if (i % 64 == 0){
                out << "    ";
            }
            QString value = QString("0x%1").arg(RGB565[i], 4, 16, QLatin1Char('0')).toUpper();
            out << value;
            if(i != RGB565.size() - 1){
                out << ", ";
            }
            if((i + 1) % 64 == 0){
                out << "\n";
            }
        }
        out << "\n};\n";
        out << "image_t " << structName
            << " = {.Buffer = " << arrayName
            << ", .Width = "    << NewImage.width()
            << ", .Height = "   << NewImage.height()
            << "};";
    }
}

void MainWindow::browseFontFile(){
    fontFilePath = QFileDialog::getOpenFileName(
        this,
        "Choose font",
        "",
        "Fonts (*.ttf)"
        );
    if(!fontFilePath.isNull()){
        refreshFontInfo();

    }

}

void MainWindow::refreshFontInfo(){
    ui->label_fontPath->setText(fontFilePath);
    int fontId = QFontDatabase::addApplicationFont(fontFilePath);
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).first();
    font.setFamily(fontFamily);
    ui->spinBox_fontSize->setValue(10);
}

void MainWindow::fontSizeChangedAction(){
    font.setPixelSize(ui->spinBox_fontSize->value());
    QFontMetrics metrics(font);
    ui->label_fontWidthVal->setText(QString::number(metrics.maxWidth()));
    ui->label_fontHeightVal->setText(QString::number(metrics.height()));
    showFontImage();
}

void MainWindow::showFontImage(){
    QImage fontImage(64,64, QImage::Format_ARGB32);
    fontImage.fill(Qt::white);
    QPainter painter(&fontImage);
    painter.setRenderHint(QPainter::TextAntialiasing, false);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.drawText(fontImage.rect(), Qt::AlignCenter, "Aa");

    ui->label_fontPreviewImage->setPixmap(
        QPixmap::fromImage(fontImage).scaled(
            ui->label_fontPreviewImage->size(),
            Qt::KeepAspectRatio,
            Qt::FastTransformation
        )
    );
}
