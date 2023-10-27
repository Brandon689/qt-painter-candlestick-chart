#pragma once

#include "config.h"
#include "typedef.h"
#include "valueaxis.h"
#include "timeaxis.h"
#include "struct.h"

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QMouseEvent>
#include <QPainterPath>
#include <QKeyEvent>

class DataSrc;

class Chart : public QObject
{
    Q_OBJECT
public:
    Chart(QObject *parent = nullptr, const int w = 200, const int h = 130);
    void paint(QPainter &p);
    QColor getbgColor();
    enum Ohlc { time, open, high, low, close, vol };
    enum Zoom { ZoomIn, ZoomOut };
    enum Scroll { Left, Right };
    bool move(Scroll dir, cd step = 1);
    bool expand(Zoom dir, cd step = 1);

    // input events reponses
    void mousePressed(const QPoint &clickpos);
    void mouseMoved(const QPoint &clickpos, const QPoint &pos, bool shift);
    void mouseReleased();
    void resized(const int w, const int h);

    friend class CWidget;

protected:
    Config *c;

private:
    ValueAxis valAxis;
    TimeAxis timeaxis;

    void updateSeriesRange();
    void fitPointsToWidth(cd &w);
    void updateSeries();

    Size size;
    Origin origin;

    // series
    QVector<QVector<d>> data;
    d gap;
    int startIndex;
    int endIndex;
    QPainterPath *path;
    QPainterPath *path2;
    d tOffset;
    d bOffset;
    DataSrc *ds;

    // internals
    d smin;
    d smax;
    d cachegap;
    int cacheminindex;
    int cachemaxindex;
    d cachemin;
    d cachemax;

    d blockOffset;
    d distance;
    d trailMousePos;

signals:
    void repaintU();
    void ca();
    void dex();
};
