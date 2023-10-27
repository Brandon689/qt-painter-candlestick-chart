#pragma once

#include <QPainter>
#include "typedef.h"
#include "struct.h"

class TimeAxis
{
public:
    TimeAxis();
    void paint(QPainter &p, Origin &ori, Size &size, int &start, int &end, d &gap, QVector<QVector<d>> &data);

private:
    int timeAxisGap;
    enum Ohlc { time, open, high, low, close, vol };
};
