#include "formsetting.h"
#include "ui_formsetting.h"

FormSetting::FormSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSetting)
{
    ui->setupUi(this);
}

FormSetting::~FormSetting()
{
    delete ui;
}
