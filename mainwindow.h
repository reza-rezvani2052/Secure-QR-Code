#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "definitions.h"

#include "simplecrypt.h"

#include "qrcodegen.h"
using namespace qrcodegen;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // از فرم تنظیمات وقتی کاربر تغییراتی اعمال میکند فراخوانی میشود
    void updateQrCode();

    void readSettings();
    void writeSettings();

    // از جاهای دیگر هم فراخوانی میشود
    void createSimpleCrypt();

private slots:
    void on_ledPlainText_textChanged(const QString &arg1);
    void on_ledEncrypted2_textChanged(const QString &arg1);

    void on_btnSave_clicked();
    void on_btnSettings_clicked();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::MainWindow *ui;

    QByteArray ba;
    SimpleCrypt *crypto;

    bool saveAsSVG(const QString &filePath);
    bool saveAsPixmap(const QByteArray &contents, const QString &filePath);

    //...

    const QrCode createQrCode(QString _text);
    const QString QrCodeToSvgString(const QrCode &qr, int border = 2, QColor color = Qt::black);

    // Prints the given QrCode object to the console.
    //void printQr(const QrCode &qr);
};

#endif // MAINWINDOW_H
