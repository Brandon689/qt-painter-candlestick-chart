#pragma once

#include <QWidget>

class CWidget;
class QPushButton;
class QComboBox;

class MWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MWidget(QWidget *parent = nullptr);

private:
    CWidget *cwidget;
    QPushButton *autoRange;
    QComboBox *dropdown;
};
