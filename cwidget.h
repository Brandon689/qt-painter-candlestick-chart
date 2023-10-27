#pragma once

#include "chart.h"

#include <QWidget>

class CWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CWidget(QWidget *parent = nullptr);
    void setChart();
    void setTheme(const QString &theme);

private:
    Chart *ch;
    bool pressed;
    QPoint clickPos;
    bool eventFilter(QObject *obj, QEvent *event);

protected:
    void paintEvent(QPaintEvent *e);
    //void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *e);

public slots:
};
