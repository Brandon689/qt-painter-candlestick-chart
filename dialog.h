#pragma once

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QColorDialog>
#include "config.h"

class CD : public QColorDialog
{
public:
    CD(QWidget *parent = nullptr) : QColorDialog(parent)
    {
        setFixedWidth(200);
    }
    void paintEvent(QPaintEvent *e)
    {
        QColorDialog::paintEvent(e);
    }
};

class CLabel : public QLabel
{
    Q_OBJECT
public:
    CLabel(QWidget *parent = nullptr) : QLabel(parent) {
       // setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
//        setMaximumSize(28, 28);
//        setMinimumHeight(28);
        setFixedSize(28, 28);
    }
    ~CLabel() {}
signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) {
        Q_UNUSED(event);
        emit clicked();
    }
};

class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(Config *config, QWidget *parent = nullptr);

    QCheckBox *borders;
    CLabel *upLabel;
    CLabel *downLabel;
    Config *config;

    void borderToggle();
    bool getColor(QColor &c, const QColor &initial);

signals:
    void settingChanged();
public slots:
    void setUpColor();
    void setDownColor();
    //void setUpWick();
    //void setDownWick();
};
