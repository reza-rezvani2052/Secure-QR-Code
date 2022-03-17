#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "simplecrypt.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ledPlainText_textChanged(const QString &arg1);
    void on_ledEncrypted2_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    SimpleCrypt *crypto;
};

#endif // MAINWINDOW_H
