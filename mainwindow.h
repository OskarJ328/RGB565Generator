#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QFont>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QString imageFilePath;
    QImage CurrImage;
    QImage NewImage;

    int NewImageWidth;
    int NewImageHeight;
    QVector<uint16_t> RGB565;


    QString fontFilePath;
    QFont font;
    void connectsInit();
    void namesInit();


    void browseImageFile();
    void showImage();
    void refreshCurrentImageInfo();
    void refreshNewImageInfo();
    void changeNewImageResolution();
    void NewImageWidthChangedAction();
    void NewImageHeightChangedAction();
    void generateNewImage();
    void saveNewImage();
    void convertToRGB565();
    void generateRGB565Array();

    void browseFontFile();
    void refreshFontInfo();
    void showFontImage();
    void fontSizeChangedAction();

};
#endif // MAINWINDOW_H
