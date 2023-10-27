#include "dialog.h"
#include <QDebug>
#include <QGridLayout>
#include <QFormLayout>
//#include <QColorDialog>
#include <QPushButton>

Dialog::Dialog(Config *config, QWidget *parent) : QDialog(parent)
{
    this->config = config;
    resize(340, 300);
    setAttribute(Qt::WA_DeleteOnClose);
    //QGridLayout *box = new QGridLayout;
    //setLayout(box);
    QFormLayout *f = new QFormLayout;
    setLayout(f);

    borders = new QCheckBox("&Borders", this);
    borders->setChecked(config->outline);

    upLabel = new CLabel;
    //upLabel->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    //upLabel->setText(config->t.up.name());
    upLabel->setPalette(QPalette(config->t.up));
    upLabel->setAutoFillBackground(true);

    downLabel = new CLabel;
    //downLabel->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    //downLabel->setText(config->t.down.name());
    downLabel->setPalette(QPalette(config->t.down));
    downLabel->setAutoFillBackground(true);

    QHBoxLayout *upDown = new QHBoxLayout;
    upDown->addWidget(upLabel);
    upDown->addWidget(downLabel);

    f->addRow(borders);
    f->addRow(tr("Candles"), upDown);

//    box->addWidget(upLabel, 0, 0);
//    box->addWidget(downLabel, 0, 1);
//    box->addWidget(borders, 1, 0);
    connect(borders, &QCheckBox::toggled, this, &Dialog::borderToggle);
    connect(upLabel, &CLabel::clicked, this, &Dialog::setUpColor);
    connect(downLabel, &CLabel::clicked, this, &Dialog::setDownColor);
}

void Dialog::borderToggle()
{
    qDebug() << borders->isChecked();
    config->outline = borders->isChecked();
    emit settingChanged();
}

void Dialog::setUpColor() {
    QColor color;
    if (getColor(color, config->t.up)) {
        upLabel->setText(color.name());
        upLabel->setPalette(QPalette(color));
        upLabel->setAutoFillBackground(true);
        config->t.up = color;
        emit settingChanged();
    }
}
void Dialog::setDownColor() {
    QColor color;
    if (getColor(color, config->t.down)) {
        upLabel->setText(color.name());
        upLabel->setPalette(QPalette(color));
        upLabel->setAutoFillBackground(true);
        config->t.down = color;
        emit settingChanged();
    }
}

bool Dialog::getColor(QColor &c, const QColor &initial) {
    //const CD::ColorDialogOptions options; //= QFlag(1); // QColorDialog::ShowAlphaChannel
    c = CD::getColor(initial, this, "Select Color", QColorDialog::ShowAlphaChannel);

    if (c.isValid())
        return true;
    else
        return false;
}

//void Dialog::setDown()
//{
//    const QColorDialog::ColorDialogOptions options; //= QFlag(1); // QColorDialog::ShowAlphaChannel
//    const QColor color = QColorDialog::getColor(Qt::green, this, "Select Color", options);

//    if (color.isValid()) {
//        downLabel->setText(color.name());
//        downLabel->setPalette(QPalette(color));
//        downLabel->setAutoFillBackground(true);

//        config->t.down = color;
//        emit settingChanged();
//    }
//}
