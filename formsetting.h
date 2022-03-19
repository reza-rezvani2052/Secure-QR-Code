#ifndef FORMSETTING_H
#define FORMSETTING_H

#include <QWidget>


namespace Ui {
class FormSetting;
}

class MainWindow;


class FormSetting : public QWidget
{
    Q_OBJECT

public:
    explicit FormSetting(QWidget *parent = nullptr);
    ~FormSetting();

private slots:
    void on_btnResetToDefault_clicked();
    void on_btnQRCodeColorChoice_clicked();
    void on_spbBorderSize_editingFinished();

private:
    Ui::FormSetting *ui;
    MainWindow *mainWindow;

    void loadAppSettings();
};

#endif // FORMSETTING_H
