#include "formchangekey.h"
#include "ui_formchangekey.h"

#include "definitions.h"

FormChangeKey::FormChangeKey(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormChangeKey)
{
    ui->setupUi(this);

    ui->spbKey->setMinimum(appSettings.QRCodeKeyMIN);
    ui->spbKey->setMaximum(appSettings.QRCodeKeyMAX);
    setKeyValue(appSettings.QRCodeKey);

    ui->spbKey->selectAll();
}

FormChangeKey::~FormChangeKey()
{
    delete ui;
}

int FormChangeKey::getKeyValue()
{
    return ui->spbKey->value();
}

void FormChangeKey::setKeyValue(const uint value)
{
    if (value >= appSettings.QRCodeKeyMIN && value <= appSettings.QRCodeKeyMAX)
        ui->spbKey->setValue(value);
    else
        ui->spbKey->setValue(appSettings.QRCodeKeyMIN);
}

void FormChangeKey::on_btnOk_clicked()
{
//    int value =  ui->spbKey->value();
//    bool isAcceptableValue =
//            value >= appSettings.QRCodeKeyMIN && value <= appSettings.QRCodeKeyMAX;

//    if (ui->spbKey->hasAcceptableInput() && isAcceptableValue)
//    {
//        accept();
//    }
//    else
//    {
//        qApp->beep();
//    }

    accept();
}

void FormChangeKey::on_btnCancel_clicked()
{
    reject();
}
