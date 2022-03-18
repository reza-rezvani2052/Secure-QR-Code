#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtDebug>

#include <QSvgWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>

//...

#include "simplecrypt.h"

//...

/*
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
*/

#include "qrcodegen.h"

//...

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ba = QByteArray();

    ui->tabWidgetEncryption->setCurrentWidget(ui->tabEncrypt);
    ui->ledEncrypted->setFocus();

    //...

    crypto = new SimpleCrypt(Q_UINT64_C(0x0c2ad4a4acb9f023)); //some random number
    //crypto->setCompressionMode(SimpleCrypt::CompressionAlways);

}

MainWindow::~MainWindow()
{
    delete crypto;
    delete ui;
}

//...

void MainWindow::on_ledPlainText_textChanged(const QString &arg1)
{
    if (ui->ledPlainText->text().isEmpty())
        ui->ledEncrypted->setText("");  //agar ledPlainText empty bashad, momken ast ledEncrypted empty nabashad!
    else
        ui->ledEncrypted->setText(crypto->encryptToString(arg1));

   const QrCode qr = createQrCode(ui->ledEncrypted->text());

   ba = QrCodeToSvgString(qr, 1).toLocal8Bit();
   ui->labelQR->load(ba);


   //printQr(qr);
}

void MainWindow::on_ledEncrypted2_textChanged(const QString &arg1)
{
    //Decryption
    QString decrypted = crypto->decryptToString(arg1);
    if (ui->ledEncrypted2->text().isEmpty() ) {
        ui->ledPlainText2->clear();
    } else if (decrypted.isEmpty()) {
        ui->ledPlainText2->setText("متن رمز شده نا معتبر است.");
    } else {
        ui->ledPlainText2->setText(decrypted);
    }
}

//...

void MainWindow::on_btnSave_clicked()
{
   if (ba.isEmpty()) {
       QMessageBox::warning(this, " ",
                                "چیزی برای ذخیره کردن وجود ندارد!");
       return;
   }

   //...

   QString filePath = QFileDialog::getSaveFileName(
               this, "ذخیره کردن" ,
               QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
               " svg پرونده(*.svg);");

   if (filePath.isEmpty()) //کاربر لغو کرده است
       return ;


   QFile file(filePath);
   file.open(QIODevice::WriteOnly);
   file.write(ba.data());
   file.close();

   //...

   /*
   //TODO: bug dareh:
   QPixmap pixmap(ui->labelQR->size());
   //ui->labelQR->render(&pixmap, QPoint(), QRegion(ui->labelQR->frameGeometry()));
   ui->labelQR->render(&pixmap, QPoint());
   //pixmap.save(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/example.png");
   ///pixmap.save(filePath + "-example.png");


   ui->labelQR->grab().save(filePath + "-example.png");
   */

   //...

   //qDebug() << ba.data();
}

const QrCode MainWindow::createQrCode(QString _text)
{
    const char *text = _text.toUtf8().constData();
    const QrCode qr = QrCode::encodeText(text, QrCode::Ecc::LOW);

    return qr;

    //printQr(qr);
    //std::cout << toSvgString(qr, 4) << std::endl;
}

// Returns a string of SVG code for an image depicting the given QR Code, with the given number
// of border modules. The string always uses Unix newlines (\n), regardless of the platform.
QString MainWindow::QrCodeToSvgString(const QrCode &qr, int border)
{
    if (border < 0)
    {
        QMessageBox::warning(this, "خطا",
                             "حاشیه کناری کیوآر کد نباید عدد منفی باشد");
        //throw std::domain_error("Border must be non-negative");
        return QString();
    }

    if (border > INT_MAX / 2 || border * 2 > INT_MAX - qr.getSize())
    {
        QMessageBox::warning(this, "خطا",
                             "حاشیه کناری کیوآر کد خیلی بزرگ است");
        //throw std::overflow_error("Border too large");
    }

    //...

    QStringList sb;
    sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
    sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
    sb << QString::number(qr.getSize() + border * 2)  << " "
       << QString::number(qr.getSize() + border * 2)  << "\" stroke=\"none\">\n";
    sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
    sb << "\t<path d=\"";
    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            if (qr.getModule(x, y)) {
                if (x != 0 || y != 0)
                    sb << " ";
                sb << "M"
                   << QString::number(x + border)
                   << "," << QString::number( y + border)
                   << "h1v1h-1z";
            }
        }
    }
    sb << "\" fill=\"#000000\"/>\n";
    sb << "</svg>\n";
    return sb.join("");
}

/*
void MainWindow::printQr(const QrCode &qr)
{
    int border = 4;
    for (int y = -border; y < qr.getSize() + border; y++) {
        for (int x = -border; x < qr.getSize() + border; x++) {
            std::cout << (qr.getModule(x, y) ? "##" : "  ");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
*/
