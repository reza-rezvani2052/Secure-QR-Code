#ifndef FORMABOUT_H
#define FORMABOUT_H

#include <QDialog>

namespace Ui {
class FormAbout;
}

class FormAbout : public QDialog
{
    Q_OBJECT

public:
    explicit FormAbout(QWidget *parent = nullptr);
    ~FormAbout();

private slots:
    void on_btnClosw_clicked();

private:
    Ui::FormAbout *ui;
};

#endif // FORMABOUT_H
