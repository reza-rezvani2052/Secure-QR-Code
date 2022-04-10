#include "formabout.h"
#include "ui_formabout.h"

#include "definitions.h"

FormAbout::FormAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormAbout)
{
    ui->setupUi(this);

    QString str = ui->label->text().replace("%VER%", AppInfo.ApplicationVersion);
    ui->label->setText(str);

    //sizeHint();
}

FormAbout::~FormAbout()
{
    delete ui;
}

void FormAbout::on_btnClosw_clicked()
{
    accept();
}
