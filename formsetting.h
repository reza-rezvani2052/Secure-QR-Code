#ifndef FORMSETTING_H
#define FORMSETTING_H

#include <QWidget>

namespace Ui {
class FormSetting;
}

class FormSetting : public QWidget
{
    Q_OBJECT

public:
    explicit FormSetting(QWidget *parent = nullptr);
    ~FormSetting();

private:
    Ui::FormSetting *ui;
};

#endif // FORMSETTING_H
