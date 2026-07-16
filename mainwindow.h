#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

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
    QString filePath;
    QImage CurrImage;
    QImage NewImage;

    int NewImageWidth;
    int NewImageHeight;
    QVector<uint16_t> RGB565;

    void connectsInit();
    void namesInit();
    void browseFile();
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
};
#endif // MAINWINDOW_H
