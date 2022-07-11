#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtDebug>
#include <QDateTime>
#include <QPainter>
#include <QSettings>
#include <QSvgWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QColorDialog>
#include <QSvgRenderer>
#include <QStandardPaths>
#include <QSystemTrayIcon>
#include <QPropertyAnimation>

#include "formabout.h"
#include "formsetting.h"
#include "formchangekey.h"

//...

#include "qrcodegen.h"
#include "simplecrypt.h"

//...

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->menuBar->hide();

    //...

    if (appSettings.isSystemTrayAvailable)
    {
        trayIcon = new QSystemTrayIcon(this);
        trayIcon->setContextMenu(ui->mnuTrayIconMenu);

        QIcon icon = QIcon(":/icons/app-icon.png");
        trayIcon->setIcon(icon);
        trayIcon->setToolTip(AppInfo.ApplicationName);

        trayIcon->show();
        //trayIcon->setVisible(AAA)
        //trayIcon->showMessage("title", "body", icon, 5 * 1000);


        connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
        //connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &MainWindow::messageClicked);
    }
    else {
        trayIcon = nullptr;
        //qDebug() << "SystemTrayIcon is not supported on this platform";
    }


    ba = QByteArray();
    ui->btnSave->setEnabled(false);
    ui->actSave->setEnabled(false);

    //...

    ui->tabWidgetEncryption->setCurrentWidget(ui->tabEncrypt);
    ui->ledEncrypted->setFocus();

    //...

    //Default settings:
    appSettings.QRCodeColor = DefaultSettings.QRCodeColor;
    appSettings.QRCodeSize = DefaultSettings.QRCodeSize;
    appSettings.QRCodeBorder = DefaultSettings.QRCodeBorder;

    appSettings.QRCodeSavePath = DefaultSettings.QRCodeSavePath;
    appSettings.QRCodeFormatType = DefaultSettings.QRCodeFormatType;

    appSettings.QRCodeKey = DefaultSettings.QRCodeKey;

    appSettings.RunAtWindowsStartup = DefaultSettings.RunAtWindowsStartup;
    appSettings.ShowAtSystemTrayOnExit = DefaultSettings.ShowAtSystemTrayOnExit;

    //...

    // اگر کاربر تغییراتی در تنظیمات داده بود؛ آن را اعمال میکنیم
    readSettings();


    crypto = nullptr;
    //crypto->setCompressionMode(SimpleCrypt::CompressionAlways);

    if (appSettings.QRCodeKey > 10000000 && appSettings.QRCodeKey <= 999999999)
    {
        //crypto = new SimpleCrypt(Q_UINT64_C(0x0c2ad4a4acb9f023)); //some random number
        crypto = new SimpleCrypt(appSettings.QRCodeKey);
    }
    //else if (appSettings.QRCodeKey == 10000000 )
    else
    {
        this->show();

        FormChangeKey form(this);
        if (form.exec() == QDialog::Accepted)
        {
            appSettings.QRCodeKey = form.getKeyValue();
            createSimpleCrypt();

            writeSettings(); // save appSettings.QRCodeKey
        }
        else
        {
            this->close();
            qApp->quit();
            //delete this;
            exit(0);
        }
    }

    //...

    ui->labelQR->setContextMenuPolicy(Qt::ContextMenuPolicy::ActionsContextMenu);
    ui->labelQR->addActions( ui->mnuSave->actions() );

    //...

}

MainWindow::~MainWindow()
{
    if (crypto != nullptr)
        delete crypto;

    delete ui;
}

//...

void MainWindow::createSimpleCrypt()
{
    if (crypto != nullptr)
        delete  crypto;

    crypto = new SimpleCrypt(appSettings.QRCodeKey);
}

//...

void MainWindow::on_ledPlainText_textChanged(const QString &arg1)
{
    if (ui->ledPlainText->text().trimmed().isEmpty()) {
        ui->ledEncrypted->setText(QString());  //agar ledPlainText empty bashad, momken ast ledEncrypted empty nabashad!

        // اگر کیوآر کدی نساخته باشیم و در فرم تنظیمات بر روی ریست کلیک کنیم
        // در کنسول کیوت کریتور وارنینگ نمایش داده میشد. البته در عملکرد برنامه تاثیر ندارد
        // مجبور شدم یک فایل اس وی جی خالی را لود کنم
        //ui->labelQR->load(QString());
        ui->labelQR->load(QString(":/empty.svg"));

        ba = QByteArray();
        ui->btnSave->setEnabled(false);
        ui->actSave->setEnabled(false);
        return ;
    } else {
        ui->ledEncrypted->setText(crypto->encryptToString(arg1));
        ui->btnSave->setEnabled(true);
        ui->actSave->setEnabled(true);
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

void MainWindow::on_actSave_triggered()
{
    on_btnSave_clicked();
}

void MainWindow::on_btnSave_clicked()
{
    if (ba.isEmpty()) {
        //TODO:  بعدا با پاپ آپ نمایش بدم
        QMessageBox::warning(this, " ",
                             "چیزی برای ذخیره کردن وجود ندارد!");
        return;
    }

    //...

    // ممکن است کاربر آخرین مسیری که ذخیره کره است را حذف کرده باشد
    QDir dir(appSettings.QRCodeSavePath);
    if (!dir.exists())
        appSettings.QRCodeSavePath = DefaultSettings.QRCodeSavePath; // Desktop path


    QString defaultFilter;
    if (appSettings.QRCodeFormatType == "png")
        defaultFilter = "عکس(*.png *.jpg)";
    else if (appSettings.QRCodeFormatType == "svg")
        defaultFilter = "svg پرونده(*.svg)";
    else
        defaultFilter = QString(); // ! دکوری هست


    QString filters("svg پرونده(*.svg);;عکس(*.png *.jpg)");

    QString strTime = QTime::currentTime().toString();
    strTime.replace(":", "-");

    QString strDate = QDate::currentDate().toString();

    QString filePath = QFileDialog::getSaveFileName(
                this, "ذخیره کردن" , appSettings.QRCodeSavePath + "/" +
                strTime + " _ " + strDate , filters, &defaultFilter);

    if (filePath.isEmpty()) //کاربر لغو کرده است
        return ;

    if (filePath.endsWith(".svg", Qt::CaseInsensitive))
    {
        saveAsSVG(filePath);
        appSettings.QRCodeFormatType = "svg";
    } else {
        saveAsPixmap(ba, filePath);
        appSettings.QRCodeFormatType = "png";
    }


    QFileInfo fileInfo( filePath );
    appSettings.QRCodeSavePath = fileInfo.dir().path();

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

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (appSettings.isSystemTrayAvailable && appSettings.ShowAtSystemTrayOnExit)
    {
        if (appSettings.numOfAppExec <= 1)
        {
            // در اجرای اول با هر بار بستن پنجره پیام نمایش داده میشد
            // کد زیر جلوی نمایش دادن چندین باره در اجرای اول را میگیرد
            appSettings.numOfAppExec++;

            trayIcon->showMessage("اجرای برنامه",
                                  "برنامه همچنان در حال اجرا است. برای پایان دادن به برنامه، گزینه 'خروج' را از منوی آیکون کنار ساعت سیستم انتخاب نمایید.",
                                  this->windowIcon() ,
                                  8 * 1000);
        }

        hide();
        e->ignore();
    }

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

    appSettings.QRCodeSavePath = settings.value("QrCodeSavePath",
                                                DefaultSettings.QRCodeSavePath).toString() ;

    appSettings.QRCodeFormatType = settings.value("QrCodeFormatType",
                                                  DefaultSettings.QRCodeFormatType).toString() ;

    appSettings.QRCodeKey = settings.value("QrCodeKey",
                                           DefaultSettings.QRCodeKey).toUInt() ;

    appSettings.numOfAppExec = settings.value("NumOfAppExec", 0).toInt() ;
    appSettings.numOfAppExec++;

    appSettings.RunAtWindowsStartup =
            settings.value("RunAtWindowsStartup", DefaultSettings.RunAtWindowsStartup).toBool();
    //TODO:  ****


    appSettings.ShowAtSystemTrayOnExit =
            settings.value("ShowAtSystemTrayOnExit", DefaultSettings.ShowAtSystemTrayOnExit).toBool();
    //TODO:  ****


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
    settings.setValue("QrCodeSavePath", appSettings.QRCodeSavePath);
    settings.setValue("QrCodeFormatType", appSettings.QRCodeFormatType);
    settings.setValue("QrCodeKey", appSettings.QRCodeKey);
    settings.setValue("NumOfAppExec", appSettings.numOfAppExec);
    settings.setValue("RunAtWindowsStartup", appSettings.RunAtWindowsStartup);
    settings.setValue("ShowAtSystemTrayOnExit", appSettings.ShowAtSystemTrayOnExit);
    //...
    settings.endGroup();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    //case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        this->setVisible( !this->isVisible() );
        break;
    case QSystemTrayIcon::MiddleClick:
        ; //Nothing!
        break;
    default:
        ;
    }
}

void MainWindow::on_actQuit_triggered()
{
    writeSettings();  //az close() estefadeh nashavad!

    if (appSettings.isSystemTrayAvailable && appSettings.ShowAtSystemTrayOnExit)
        trayIcon->hide();

    qApp->quit();
    exit(0);
}

void MainWindow::on_actAboutApp_triggered()
{
    ui->actAboutApp->setEnabled(false);

    if (!this->isVisible())
        this->setVisible(true);

    //FIXME: اگر ماکسیمایز باشه و بعد مینیمایز شده باشه........
    if (this->isMinimized())
        showNormal();

    FormAbout form(this);
    form.exec();

    ui->actAboutApp->setEnabled(true);
}

void MainWindow::on_tabWidgetEncryption_currentChanged(int index)
{
    Q_UNUSED(index)

    animateTabWidgetPages();
}

void MainWindow::animateTabWidgetPages(Direction dir)
{
    QWidget *page = ui->tabWidgetEncryption->currentWidget();

    const QPoint posEnd = page->pos();
    QPoint posStart = posEnd;

    switch (dir) {
    case Direction::TopToBottom :
        posStart.setX(0);
        posStart.setY(35);
        break;
    case Direction::BottomToTop :
        posStart.setX(0);
        posStart.setY(-35);
        break;
    case Direction::RightToLeft :
        posStart.setX(50);
        posStart.setY(0);
        break;
    case Direction::LeftToRight :
        posStart.setX(-50);
        posStart.setY(0);
        break;
    }


    QPropertyAnimation *animation = new QPropertyAnimation(page, "pos");

    animation->setDuration(100);
    animation->setStartValue(posStart);
    animation->setEndValue(posEnd);

    animation->setEasingCurve(QEasingCurve::Linear /*OutElastic*/);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}
