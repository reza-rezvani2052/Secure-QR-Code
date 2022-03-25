#include "formchangekey.h"
#include "ui_formchangekey.h"

FormChangeKey::FormChangeKey(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormChangeKey)
{
    ui->setupUi(this);
    ui->spinBox->value();
}

FormChangeKey::~FormChangeKey()
{
    delete ui;
}

int FormChangeKey::getKeyValue()
{
    return ui->spinBox->value();
}

void FormChangeKey::setKeyValue(const int value)
{
    //TODO: بعدا صحت و محدودیت ورودی ها را تست کنم
    ui->spinBox->setValue(value);
}

void FormChangeKey::on_btnOk_clicked()
{
    accept();
}

void FormChangeKey::on_btnCancel_clicked()
{
    reject();
}
