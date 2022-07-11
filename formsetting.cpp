#include "formsetting.h"
#include "ui_formsetting.h"

#include <QColorDialog>

#include "mainwindow.h"
#include "definitions.h"
#include "formchangekey.h"

FormSetting::FormSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSetting)
{
    ui->setupUi(this);
    //...
    setWindowFlags(Qt::Popup);
    setAttribute(Qt::WA_DeleteOnClose);
    //...

    // حافظه مربوط به این اشاره گر را نباید دلیت کنم
    mainWindow = qobject_cast<MainWindow *>( parent );

    //...
    loadAppSettings();
    //...

    adjustSize();
}

FormSetting::~FormSetting()
{
    //mainWindow->updateQrCode();
    delete ui;
}

void FormSetting::loadAppSettings()
{
    ui->spbBorderSize->setValue(appSettings.QRCodeBorder);
    //...
    ui->spbKey->setValue(appSettings.QRCodeKey);
    //...
    ui->spbQRCodeSize->setValue(appSettings.QRCodeSize);
    //...
    QColor col = appSettings.QRCodeColor;
    if(col.isValid())
    {
       QString qss = QString("background-color: %1").arg(col.name());
       ui->btnQRCodeColorChoice->setStyleSheet(qss);
    }
    //...
    ui->chkShowAtSystemtray->setChecked(appSettings.ShowAtSystemTrayOnExit);
    ui->chkRunAtWindowsStartup->setChecked(appSettings.RunAtWindowsStartup);

}

void FormSetting::on_btnQRCodeColorChoice_clicked()
{
    QColorDialog colorDialog(this);
    colorDialog.setWindowTitle("انتخاب رنگ");
    colorDialog.setLayoutDirection(Qt::LeftToRight);

    QColor color = appSettings.QRCodeColor;
    colorDialog.setCurrentColor(color);

    if ( colorDialog.exec() == QDialog::Accepted ) {
        appSettings.QRCodeColor = colorDialog.currentColor(); //.name();

        loadAppSettings();

        mainWindow->updateQrCode();
    }
}

void FormSetting::on_spbBorderSize_editingFinished()
{
    const int borderSize = ui->spbBorderSize->value();
    appSettings.QRCodeBorder = borderSize;

    loadAppSettings();

    mainWindow->updateQrCode();
}

void FormSetting::on_spbQRCodeSize_editingFinished()
{
    const int QrCodeSize = ui->spbQRCodeSize->value();
    appSettings.QRCodeSize = QrCodeSize;

    // چون سایز در زمان ذخیره شدن اعمال میشود نیازی به این نیست
    //loadAppSettings();

    mainWindow->updateQrCode();
}

void FormSetting::on_btnResetToDefault_clicked()
{
    appSettings.QRCodeBorder = DefaultSettings.QRCodeBorder;
    appSettings.QRCodeSize   = DefaultSettings.QRCodeSize  ;
    appSettings.QRCodeColor  = DefaultSettings.QRCodeColor ;

    loadAppSettings();

    mainWindow->updateQrCode();
}

void FormSetting::on_btnChangeKey_clicked()
{
    FormChangeKey form(mainWindow);
    if (form.exec() == QDialog::Accepted)
    {
        appSettings.QRCodeKey = form.getKeyValue();
        mainWindow->createSimpleCrypt();

        mainWindow->writeSettings(); // save appSettings.QRCodeKey
        mainWindow->updateQrCode();
    }
}

void FormSetting::on_chkShowAtSystemtray_toggled(bool checked)
{
    //TODO: ***********
}
