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
    struct AppSettings {
        int    QRCodeBorder;  // = 2;
        QColor  QRCodeColor;  // = Qt::black;

        // مقادیر پیش فرض در ساختار زیر تعریف شده اند تغییرات در این ساختار را در فایل
        // definitions
        // اعمال نمایم
        //AppDefaultSettings DefaultSettings;
    } appSettings;

    // چون در دیالوگ ستینگز به این نیاز داشتیم، عمومی تعریف کردم
    Ui::MainWindow *ui;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // از فرم تنظیمات وقتی کاربر تغییراتی اعمال میکند فراخوانی میشود
    void updateQrCode();

private slots:
    void on_ledPlainText_textChanged(const QString &arg1);
    void on_ledEncrypted2_textChanged(const QString &arg1);

    void on_btnSave_clicked();

    void on_btnSettings_clicked();

protected:
    void closeEvent(QCloseEvent *);

private:
    QByteArray ba;
    SimpleCrypt *crypto;

    QString m_desktopPath;

    bool saveMethod1(const QString &filePath);
    bool saveMethod2(const QString &filePath);

    void readSettings();
    void writeSettings();
    //...

    const QrCode createQrCode(QString _text);
    const QString QrCodeToSvgString(const QrCode &qr, int border = 2, QColor color = Qt::black);

    // Prints the given QrCode object to the console.
    //void printQr(const QrCode &qr);
};

#endif // MAINWINDOW_H
