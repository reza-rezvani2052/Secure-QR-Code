#include "formchangekey.h"
#include "ui_formchangekey.h"

#include "definitions.h"

FormChangeKey::FormChangeKey(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormChangeKey)
{
    ui->setupUi(this);
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

void FormChangeKey::setKeyValue(const int value)
{
    //TODO: بعدا صحت و محدودیت ورودی ها را تست کنم
    ui->spbKey->setValue(value);
}

void FormChangeKey::on_btnOk_clicked()
{
    accept();
}

void FormChangeKey::on_btnCancel_clicked()
{
    reject();
}
