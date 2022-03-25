#ifndef FORMCHANGEKEY_H
#define FORMCHANGEKEY_H

#include <QDialog>

namespace Ui {
class FormChangeKey;
}

class FormChangeKey : public QDialog
{
    Q_OBJECT

public:
    explicit FormChangeKey(QWidget *parent = nullptr);
    ~FormChangeKey();

    int getKeyValue();
    void setKeyValue(const int value);

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

private:
    Ui::FormChangeKey *ui;
};

#endif // FORMCHANGEKEY_H
