#include "mwidget.h"
#include "cwidget.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>
#include <QSizePolicy>

MWidget::MWidget(QWidget *parent) : QWidget(parent)
{
    resize(800, 600);
    setContentsMargins(0,0,0,0);
    cwidget = new CWidget(this);
    cwidget->setChart();
    autoRange = new QPushButton("Auto Range");
    autoRange->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
    dropdown = new QComboBox;
    dropdown->addItem("Light");
    dropdown->addItem("Dark");
    dropdown->addItem("Sand");
//    dropdown->addItem("Night");
//    dropdown->addItem("Meta Green");
//    dropdown->addItem("Icy");
//    dropdown->addItem("Nature");
//    dropdown->addItem("Batman");
    dropdown->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

    cwidget->setTheme("Sand");

    //connect(dropdown, QOverload<const QString &>::of(&QComboBox::activated), [=](const QString &text)
    //{
    //    squid->setTheme(text);
    //});

    autoRange->setFocusPolicy(Qt::NoFocus);
    dropdown->setFocusPolicy(Qt::NoFocus);

    QVBoxLayout *v = new QVBoxLayout;
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(dropdown);
    hbox->addWidget(autoRange);
    v->setContentsMargins(0,0,0,0);
    v->setSpacing(0);
    //v->addWidget(dropdown);
    //v->addWidget(autoRange);
    v->addLayout(hbox);
    v->addWidget(cwidget);
    setLayout(v);
    installEventFilter(cwidget);

    //connect(autoRange, &QPushButton::pressed, squid, &Squid::autoRangeToggle);
}
