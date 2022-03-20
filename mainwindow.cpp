#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtDebug>

#include <QPainter>
#include <QSettings>
#include <QSvgWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QSvgRenderer>
#include <QStandardPaths>

#include "formsetting.h"

//...

#include "qrcodegen.h"
#include "simplecrypt.h"

//...

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ba = QByteArray();
    ui->btnSave->setEnabled(false);

    crypto = new SimpleCrypt(Q_UINT64_C(0x0c2ad4a4acb9f023)); //some random number
    //crypto->setCompressionMode(SimpleCrypt::CompressionAlways);

    //...

    ui->tabWidgetEncryption->setCurrentWidget(ui->tabEncrypt);
    ui->ledEncrypted->setFocus();

    //...

    //Default settings:
    appSettings.QRCodeColor = DefaultSettings.QRCodeColor;
    appSettings.QRCodeSize = DefaultSettings.QRCodeSize;
    appSettings.QRCodeBorder = DefaultSettings.QRCodeBorder;

    // اگر کاربر تغییراتی در تنظیمات داد؛ آن را اعمال میکنیم
    readSettings();

    //...

}

MainWindow::~MainWindow()
{
    delete crypto;
    delete ui;
}

//...

void MainWindow::on_ledPlainText_textChanged(const QString &arg1)
{
    if (ui->ledPlainText->text().trimmed().isEmpty()) {
        ui->ledEncrypted->setText(QString());  //agar ledPlainText empty bashad, momken ast ledEncrypted empty nabashad!
        ui->labelQR->load(QString());
        ba = QByteArray();
        ui->btnSave->setEnabled(false);
        return ;
    } else {
        ui->ledEncrypted->setText(crypto->encryptToString(arg1));
        ui->btnSave->setEnabled(true);
    }

   const QrCode qr = createQrCode(ui->ledEncrypted->text());

   ba = QrCodeToSvgString(qr, appSettings.QRCodeBorder,
                          appSettings.QRCodeColor).toLocal8Bit();
   ui->labelQR->load(ba);
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

   QString filters("svg پرونده(*.svg);;عکس(*.png *.jpg)");
   QString defaultFilter("عکس(*.png *.jpg)");

   QString filePath = QFileDialog::getSaveFileName(
                         this, "ذخیره کردن" ,
                         QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                         filters, &defaultFilter);

   if (filePath.isEmpty()) //کاربر لغو کرده است
       return ;

   if (filePath.endsWith(".svg", Qt::CaseInsensitive))
   {
       saveAsSVG(filePath);
   } else {
       saveAsPixmap(ba, filePath);
   }

   //...
   //qDebug() << ba.data();
}

bool MainWindow::saveAsSVG(const QString &filePath)
{
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    quint64 ret = file.write(ba.data());
    file.close();
    return (int)ret == -1 ? false : true ;
}

bool MainWindow::saveAsPixmap(const QByteArray &contents, const QString &filePath)
{
    QSvgRenderer renderer(contents);
    QPixmap pm( QSize(appSettings.QRCodeSize, appSettings.QRCodeSize ) );  // renderer.defaultSize()
    QPainter painter(&pm);
    renderer.render(&painter, pm.rect());  // renderer.render( &painter );
    return pm.save(filePath);
}

void MainWindow::updateQrCode()
{
    on_ledPlainText_textChanged(ui->ledPlainText->text());
}

const QrCode MainWindow::createQrCode(QString _text)
{
    const char *text = _text.toUtf8().constData();
    const QrCode qr = QrCode::encodeText(text, QrCode::Ecc::LOW);

    return qr;

    //printQr(qr);
}

// Returns a string of SVG code for an image depicting the given QR Code, with the given number
// of border modules. The string always uses Unix newlines (\n), regardless of the platform.
const QString MainWindow::QrCodeToSvgString(const QrCode &qr, int border, QColor color)
{
    if (border < 0)
    {
        QMessageBox::warning(this, "خطا",
                             "حاشیه کناری کیوآر کد نباید عدد منفی باشد");
        return QString();
    }

    if (border > INT_MAX / 2 || border * 2 > INT_MAX - qr.getSize())
    {
        QMessageBox::warning(this, "خطا",
                             "حاشیه کناری کیوآر کد خیلی بزرگ است");
        return QString();
    }

    //...

    QStringList sl;
    sl << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    sl << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
    sl << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
    sl << QString::number(qr.getSize() + border * 2)  << " "
       << QString::number(qr.getSize() + border * 2)  << "\" stroke=\"none\">\n";
    sl << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
    sl << "\t<path d=\"";
    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            if (qr.getModule(x, y)) {
                if (x != 0 || y != 0)
                    sl << " ";
                sl << "M"
                   << QString::number(x + border)
                   << "," << QString::number( y + border)
                   << "h1v1h-1z";
            }
        }
    }
    //sl << "\" fill=\"#000000\"/>\n";
    //sl << "\" fill=\"#" << "COLOR_CODE_GOES_HERE" << "\" stroke-width=\"0\"/>\n";
    //sl << "\" fill=\"" << "COLOR_NAME_GOES_HERE" << "\" stroke-width=\"0\"/>\n";
    sl << "\" fill=\"" << color.name() << "\" stroke-width=\"0\"/>\n";
    sl << "</svg>\n";

    return sl.join("");
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

void MainWindow::on_btnSettings_clicked()
{
    FormSetting *dialog = new FormSetting(this);
    QPoint xy = ui->btnSettings->mapToGlobal( QPoint(0,0) );
    xy.setX( xy.x() - dialog->width() + ui->btnSettings->width() );
    xy.setY( xy.y() - dialog->height() - 2 );
    dialog->move(xy);
    dialog->show();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    writeSettings();
}

void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    //...
    restoreGeometry(settings.value("Geometry").toByteArray());

    int borderSize = settings.value("QrCodeBorderSize",
                                    DefaultSettings.QRCodeBorder).toInt() ;
    if (borderSize >= 0 && borderSize <= 10)
        appSettings.QRCodeBorder = borderSize;
    else
        appSettings.QRCodeBorder = DefaultSettings.QRCodeBorder;

    int QrCodeSize = settings.value("QrCodeSize",
                                    DefaultSettings.QRCodeSize).toInt() ;
    if (QrCodeSize >= 25 && QrCodeSize <= 900)
        appSettings.QRCodeSize = QrCodeSize;
    else
        appSettings.QRCodeSize = DefaultSettings.QRCodeSize;

    QColor color = settings.value("QrCodeColor", DefaultSettings.QRCodeColor).value<QColor>();
    appSettings.QRCodeColor = color;

    //...
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    //...
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("QrCodeBorderSize", appSettings.QRCodeBorder );
    settings.setValue("QrCodeSize", appSettings.QRCodeSize );
    settings.setValue("QrCodeColor", appSettings.QRCodeColor);
    //...
    settings.endGroup();
}

