#include "chart.h"
#include "util.h"
#include "datasrc.h"

#include <QPainter>
#include <QDebug>

Chart::Chart(QObject *parent, const int w, const int h) : QObject(parent),
    valAxis(ValueAxis::Right), gap(18), startIndex(0), tOffset(0), bOffset(0)
{
    c = new Config;
    // Feed data
    ds = new DataSrc;
    ds->readAlpha("./GE.csv", data);
    //ds->readJson("./kline4h.json", data);

    // Dimensions
    size = {w - c->xmargin, h - c->ymargin};

    if (valAxis.orient == ValueAxis::Right)
        origin = {0.0, static_cast<d>(h - c->ymargin)};
    else
        origin = {static_cast<d>(c->xmargin), static_cast<d>(h - c->ymargin)};

    // Init ranges
    endIndex = data.length();
    distance = 0;
    trailMousePos = 0;
    blockOffset = 0;
}

void Chart::paint(QPainter &p)
{
    //qDebug() << p.font().pointSize();
    p.setPen(QColor(155, 153, 161));
    p.drawLine(0, 0, size.w + c->xmargin, 0);
    p.setPen(c->t.axis);

    valAxis.paint(p, origin, size);
    timeaxis.paint(p, origin, size, startIndex, endIndex, gap, data);

    p.setClipRect( QRect(origin.x, 0, size.w, size.h) );
    if (c->type == Config::LINE) {
        p.setPen(c->t.lineSeries);
        p.setRenderHint(QPainter::Antialiasing);
        p.drawPath(*path);
    }
    else if (c->type == Config::CANDLE) {
        if (!c->outline) {
            p.setPen(c->t.down);
            p.setBrush(c->t.down);
            p.drawPath(*path);

            p.setPen(c->t.up);
            p.setBrush(c->t.up);
            p.drawPath(*path2);
        } else {
            p.setPen(c->t.upOutline);
            p.setBrush(c->t.down);
            p.drawPath(*path);
            p.setPen(c->t.downOutline);
            p.setBrush(c->t.up);
            p.drawPath(*path2);
        }
    }
}

QColor Chart::getbgColor() {
    return c->t.bg;
}

void Chart::fitPointsToWidth(cd &w)
{
    startIndex = endIndex - (w / gap);

    if (startIndex < 0)
        startIndex = 0;

    d space = (endIndex - startIndex) * gap;
    if (space < size.w)
    {
        d dif = size.w - space;
        d dex = dif / gap;
        endIndex += dex;
        if (endIndex > data.length())
            endIndex = data.length();
    }
}

void Chart::updateSeriesRange()
{
    smin = data[startIndex][low];
    smax = 0;
    for (int i = startIndex; i < endIndex; ++i)
    {
        if (smin > data[i][low])
            smin = data[i][low];
        if (smax < data[i][high])
            smax = data[i][high];
    }
}

bool Chart::move(Scroll dir, cd step)
{
    bool succeed = false;
    if (dir == Chart::Right) {
        if (endIndex < data.length()) {
            endIndex += step;
            startIndex += step;
            succeed = true;
        }
    } else {
        if (startIndex > 0) {
            endIndex -= step;
            startIndex -= step;
            succeed = true;
        }
    }
    if (succeed) {
        if (c->autoAlignPointCenter) {
            distance = 0;
            blockOffset = distance;
        }
        emit dex(); // index change lol rename it..
        updateSeriesRange();
        valAxis.recalculateRange(smin, smax, tOffset, bOffset);
        valAxis.setAxisLabels(size);
        updateSeries();
        emit repaintU();
    }
    return succeed;
//    qDebug() << "td";
//    valAxis.moved(origin, false, 20, 40, smax, smin, size);
//    updateSeriesRange();
//    //valAxis.recalculateRange(smin, smax, tOffset, bOffset);
//    valAxis.setAxisLabels(size);
//    updateSeries();
//    emit repaintU();
//    return true;
}

bool Chart::expand(Zoom dir, cd step)
{
    bool succeed = false;
    const int nPointsOld = endIndex - startIndex;
    if (dir == Chart::ZoomOut) {
        if (endIndex < data.length()) {
            endIndex += step;
            succeed = true;
        }
        if (startIndex > 0) {
            startIndex -= step;
            succeed = true;
        }
    } else {
       if (startIndex + c->minPoints < endIndex) {
           startIndex += step;
           //endIndex -= step; dont move the leader
           succeed = true;
       }
    }
    if (succeed) {
        if (c->autoAlignPointCenter) {
            distance = 0;
            blockOffset = distance;
        }
        emit dex(); // index changed signal
        updateSeriesRange();
        valAxis.recalculateRange(smin, smax, tOffset, bOffset);

        const int diff = nPointsOld - (endIndex - startIndex);
        gap += (gap / (endIndex - startIndex)) * diff;

        valAxis.setAxisLabels(size);
        updateSeries();
        emit repaintU();
    }
    return succeed;
}

void Chart::updateSeries() {
    int si = startIndex;
    int ei = endIndex;

    if (startIndex > 0)
        --si;
    if (endIndex < data.length())
        ++ei;

    path = new QPainterPath;
    path2 = new QPainterPath;

    if (c->type == Config::LINE) {
        for (int i = 0; i < (ei - si); ++i)
            path->lineTo(QPointF(origin.x + gap * i, toCoord(data[i + startIndex][open],
                                 origin.y, valAxis.getMax(), valAxis.getMax())));
    }
    else if (c->type == Config::CANDLE) {
        const int wr = c->candleW / 2;

        for (int i = si; i < ei; ++i)
        {
            d o, h, l, c;
            if (valAxis.scale == ValueAxis::Linear) {
                o = toCoord(data[i][open], origin.y, valAxis.getMax(), valAxis.getMin());
                h = toCoord(data[i][high], origin.y, valAxis.getMax(), valAxis.getMin());
                l = toCoord(data[i][low], origin.y, valAxis.getMax(), valAxis.getMin());
                c = toCoord(data[i][close], origin.y, valAxis.getMax(), valAxis.getMin());
            }
            else {
                o = toLogCoord(data[i][open], origin.y, valAxis.getMax(), valAxis.getMin());
                h = toLogCoord(data[i][high], origin.y, valAxis.getMax(), valAxis.getMin());
                l = toLogCoord(data[i][low], origin.y, valAxis.getMax(), valAxis.getMin());
                c = toLogCoord(data[i][close], origin.y, valAxis.getMax(), valAxis.getMin());
            }

            d x = origin.x + (gap * (i - startIndex)) - distance;

            // close > open version
            if (data[i][close] > data[i][open]) {
                path->addRect(QRectF(QPointF(x - wr, o), QPointF(x + wr, c)));
                path->moveTo(x, h);
                path->lineTo(x, c);
                path->moveTo(x, l);
                path->lineTo(x, o);
            }
            else {
                path2->addRect(QRectF(QPointF(x - wr, o), QPointF(x + wr, c)));
                path2->moveTo(x, h);
                path2->lineTo(x, o);
                path2->moveTo(x, l);
                path2->lineTo(x, c);
            }
        }
    }
}

void Chart::resized(const int w, const int h)
{
    size.w = w - c->xmargin;
    size.h = h - c->ymargin;
    origin.y = static_cast<d>(size.h);

    fitPointsToWidth(size.w);
    updateSeriesRange();
    valAxis.recalculateRange(smin, smax, tOffset, bOffset);
    valAxis.setAxisLabels(size);
    updateSeries();
}

void Chart::mousePressed(const QPoint &clickpos) {
    valAxis.setClickMinMax();
    cacheminindex = startIndex;
    cachemaxindex = endIndex;
    trailMousePos = clickpos.x();
}

void Chart::mouseMoved(const QPoint &clickpos, const QPoint &pos, bool shift)
{
    if (!shift) {
        distance = trailMousePos - pos.x() + blockOffset;

        if (distance > 0 && endIndex < data.length())
        {
            if (distance >= gap) // cursor left
            {
                trailMousePos = pos.x();
                distance = 0;
                blockOffset = 0;
                ++startIndex;
                ++endIndex;
            }
        }
        else if (distance < 0 && startIndex > 0)
        {
            if (distance <= -gap) // cursor right
            {
                trailMousePos = pos.x();
                distance = 0;
                blockOffset = 0;
                --startIndex;
                --endIndex;
            }
        } else {
            distance = 0;
            trailMousePos = pos.x();
        }
    }
    // min max
    valAxis.moved(origin, shift, clickpos.y() - pos.y(), clickpos.y(), smax, smin, size);

    updateSeriesRange();
    valAxis.setAxisLabels(size);
    updateSeries();
    emit repaintU();
}

void Chart::mouseReleased() {
    blockOffset = distance;
    cd range = smax - smin;
    tOffset = (valAxis.getMin() - smin) / range;
    bOffset = (valAxis.getMax() - smax) / range;
}
