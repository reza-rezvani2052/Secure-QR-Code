#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void on_ledPlainText_textChanged(const QString &arg1);
    void on_ledEncrypted2_textChanged(const QString &arg1);

    void on_btnSave_clicked();

private:
    Ui::MainWindow *ui;
    SimpleCrypt *crypto;

    QByteArray ba;

    //...

    const QrCode createQrCode(QString _text);

    QString QrCodeToSvgString(const QrCode &qr, int border);

    // Prints the given QrCode object to the console.
    //void printQr(const QrCode &qr);
};

#endif // MAINWINDOW_H