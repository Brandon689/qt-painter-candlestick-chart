#pragma once

#include <QPainter>
#include "typedef.h"
#include "struct.h"

class ValueAxis
{
public:
    enum Orientation { Left, Right };
    enum Scale { Linear, Log };

    ValueAxis(Orientation orientation);
    void paint(QPainter &p, Origin &ori, Size &size);
    void setAxisLabels(Size &size);
    void recalculateRange(cd &smin, cd &smax, cd &topOffset, cd &botOffset);
    void moved(Origin &ori, bool shift, cd &moveAmount, cd &clickpos, cd &smax, cd &smin, Size &size);
    void setClickMinMax();
    void setScale(Scale s) { scale = s; }

    void setMin(cd &m) { min = m; }
    void setMax(cd &m) { max = m; }

    d getMin() { return min; }
    d getMax() { return max; }

    void setAutoRange() { autoRange = !autoRange; }
    Orientation orient;
    Scale scale;

private:
    bool autoRange;
    d min;
    d max;
    d clickmin;
    d clickmax;
    int ticks;
    QVector<d> axisLabel;
};
