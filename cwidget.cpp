#include "cwidget.h"
#include "dialog.h"

#include <QGuiApplication>
#include <QKeyEvent>
#include <QPainter>
#include <QDebug>
#include <QDialog>


CWidget::CWidget(QWidget *parent) : QWidget(parent), ch(nullptr), pressed(false)
{
    resize(800, 600);
}

void CWidget::setTheme(const QString &theme)
{
    ch->c->theme = theme;
    ch->c->themeChanged();
    update();
}

void CWidget::setChart()
{
    if (ch == nullptr) {
        ch = new Chart(this, width(), height());
        connect(ch, SIGNAL(repaintU()), this, SLOT(update()));
    }
}

bool CWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        //qDebug() << "DAM";
        QKeyEvent *e = static_cast<QKeyEvent *>(event);

        if (pressed == false) {
            if (e->key() == Qt::Key_Left) {
                ch->move(Chart::Left);
            }
            else if (e->key() == Qt::Key_Right) {
                ch->move(Chart::Right);
            }
            else if (e->key() == Qt::Key_Equal) {
                ch->expand(Chart::ZoomIn);
            }
            else if (e->key() == Qt::Key_Minus) {
                ch->expand(Chart::ZoomOut);
            }
        }
        if (e->key() == Qt::Key_D)
        {
            qDebug() << "im";
            Dialog *d = new Dialog(ch->c, this);
            //connect(d, &Dialog::settingChanged, this, &QWidget::update);
            connect(d, SIGNAL(settingChanged()), this, SLOT(update()));
            d->show();
        }
        return true;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

void CWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter p(this);
    p.fillRect(rect(), ch->getbgColor());
    ch->paint(p);
}

//void Squid::keyPressEvent(QKeyEvent *e) {
//    if (pressed == false) {
//        if (e->key() == Qt::Key_Left) {
//            ch->move(Chart::Left);
//        }
//        else if (e->key() == Qt::Key_Right) {
//            ch->move(Chart::Right);
//        }
//        else if (e->key() == Qt::Key_Equal) {
//            ch->expand(Chart::ZoomIn);
//        }
//        else if (e->key() == Qt::Key_Minus) {
//            ch->expand(Chart::ZoomOut);
//        }
//    }
//}

void CWidget::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        pressed = true;
        clickPos = e->pos();
        ch->mousePressed(clickPos);
    }
}

void CWidget::mouseMoveEvent(QMouseEvent *e) {
    if (pressed) {
        if (QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ShiftModifier))
            ch->mouseMoved(clickPos, e->pos(), true);
        else
            ch->mouseMoved(clickPos, e->pos(), false);
    }
}

void CWidget::mouseReleaseEvent(QMouseEvent *e) {
    Q_UNUSED(e);
    pressed = false;
    ch->mouseReleased();
}

void CWidget::resizeEvent(QResizeEvent *e)
{
    //qDebug() << width();
    QWidget::resizeEvent(e);
    ch->resized(width(), height());
}
