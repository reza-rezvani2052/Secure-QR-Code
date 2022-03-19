#include "formsetting.h"
#include "ui_formsetting.h"

#include <QColorDialog>

#include "mainwindow.h"

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
    ui->spbBorderSize->setValue(mainWindow->appSettings.QRCodeBorder);
    //...
    QColor col = mainWindow->appSettings.QRCodeColor;
    if(col.isValid())
    {
       QString qss = QString("background-color: %1").arg(col.name());
       ui->btnQRCodeColorChoice->setStyleSheet(qss);
    }
    //...
}

void FormSetting::on_btnQRCodeColorChoice_clicked()
{
    QColorDialog colorDialog(this);
    colorDialog.setWindowTitle("انتخاب رنگ");
    colorDialog.setLayoutDirection(Qt::LeftToRight);

    QColor color = mainWindow->appSettings.QRCodeColor;
    colorDialog.setCurrentColor(color);

    if ( colorDialog.exec() == QDialog::Accepted ) {
        mainWindow->appSettings.QRCodeColor = colorDialog.currentColor(); //.name();

        loadAppSettings();

        mainWindow->updateQrCode();
    }
}

void FormSetting::on_spbBorderSize_editingFinished()
{
    const int borderSize = ui->spbBorderSize->value();
    mainWindow->appSettings.QRCodeBorder = borderSize;

    loadAppSettings();

    mainWindow->updateQrCode();
}

void FormSetting::on_btnResetToDefault_clicked()
{
    mainWindow->appSettings.QRCodeBorder = DefaultSettings.QRCodeBorder;
    mainWindow->appSettings.QRCodeColor = DefaultSettings.QRCodeColor;

    loadAppSettings();

    mainWindow->updateQrCode();
}
