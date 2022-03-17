#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "simplecrypt.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tabWidgetEncryption->setCurrentWidget(ui->tabEncrypt);
    //...
    crypto = new SimpleCrypt(Q_UINT64_C(0x0c2ad4a4acb9f023)); //some random number
    //crypto->setCompressionMode(SimpleCrypt::CompressionAlways);
}

MainWindow::~MainWindow()
{
    delete crypto;
    delete ui;
}


void MainWindow::on_ledPlainText_textChanged(const QString &arg1)
{
    //Encryption
    ui->ledEncrypted->setText(crypto->encryptToString(arg1));
}

void MainWindow::on_ledEncrypted2_textChanged(const QString &arg1)
{
    //Decryption
    QString decrypted = crypto->decryptToString(arg1);
    ui->ledPlainText2->setText(decrypted);
}
