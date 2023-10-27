#include "valueaxis.h"
#include "nicetick.h"
#include "util.h"
#include <QDebug>
#include <QtMath>

static d magnitude;

ValueAxis::ValueAxis(Orientation orientation)
{
    ticks = 0;
    min = 0;
    max = 0;
    clickmin = 0;
    clickmax = 0;
    autoRange = false;
    orient = orientation;
    scale = Log;
}

void ValueAxis::setAxisLabels(Size &size) {
    ticks = 16;

    if (size.h < 400)
        ticks = 6;
    d t_min = min;
    d t_max = max;
    looseNiceNumbers(t_min, t_max, ticks);

    cd vgap = (t_max - t_min) / (static_cast<d>(ticks) - 1);
    if (t_max > max) {
        --ticks;
    }
    if (t_min < min) {
        --ticks;
        t_min += vgap;
    }
    if (axisLabel.size() != ticks)
        axisLabel.resize(ticks);

    for (int i = 0; i < ticks; ++i)
        axisLabel[i] = t_min + (i * vgap);
}

void ValueAxis::moved(Origin &ori, bool shift, cd &moveAmount, cd &clickpos, cd &smax, cd &smin, Size &size)
{
    if (autoRange) {
        min = smin;
        max = smax;
    } else {
        if (scale == Log)
        {
            d mo = moveAmount;
            d slmin = std::log10(clickmin);
            d slmax = std::log10(clickmax);
            d cake = (1.0 - (slmin / slmax));
            d crum = size.h / cake;

            if (shift)
            {
                double coord = size.h - mo;
                d norm = 1.0 - coord / crum;

                d lessAmount = qPow(clickmax, norm) - clickmin;

                min = clickmin + lessAmount;

                d tcoord = mo;
                d s2 = 1.0 - (tcoord / crum);
                d moreAmount = qPow(clickmax, s2) - clickmax;
                qDebug() << clickmax + moreAmount;

                max = clickmax + moreAmount;

                magnitude = min / max;


            } else {
                d mov = moveAmount;
                cd s2 = 1 - (mov / crum);

                d moreAmount = qPow(clickmax, s2) - clickmax;
                max = clickmax + moreAmount;
                min = max * magnitude;
            }
        }
        else {
            cd move = toValue(moveAmount, ori.y, max, min);
            max = clickmax - move;
            if (shift)
                min = clickmin + move;
            else
                min = clickmin - move;
        }
    }
}

void ValueAxis::setClickMinMax() {
    clickmin = min;
    clickmax = max;
}

void ValueAxis::recalculateRange(cd &smin, cd &smax, cd &topOffset, cd &botOffset) {
    min = smin + ((smax - smin) * topOffset);
    max = smax + ((smax - smin) * botOffset);
    magnitude = min / max;
}

void ValueAxis::paint(QPainter &p, Origin &ori, Size &size) {
    double slmin = std::log10(min);
    double slmax = std::log10(max);

    if (scale == Log)
    {
        p.drawLine(size.w, 0, size.w, size.h);
        for (int i = 0; i < ticks; ++i)
        {
            cd cake = (1 - (slmin / slmax));
            cd crum = size.h / cake;

            cd s = std::log10(axisLabel[i]) / std::log10(max); // number with base max (0 to 1)
            cd ret = (1 - s) * crum;

            p.drawLine(size.w, ret, size.w + 10, ret);
            p.drawText(QPointF(size.w + 20, ret), QString::number(axisLabel[i], 'f', 1));
        }
    }
    else {
        if (orient == ValueAxis::Right)
        {
            p.drawLine(size.w, 0, size.w, size.h);
            for (int i = 0; i < ticks; ++i)
            {
                cd yp = toCoord(axisLabel[i], ori.y, max, min);
                p.drawLine(size.w, yp, size.w + 10, yp);
                p.drawText(QPointF(size.w + 20, yp), QString::number(axisLabel[i], 'f', 1));
            }
        } else {
            p.drawLine(ori.x, 0, ori.x, size.h);

            for (int i = 0; i < ticks; ++i)
            {
                cd yp = toCoord(axisLabel[i], ori.y, max, min);
                p.drawLine(ori.x, yp, ori.x - 10, yp);
                p.drawText(QPointF(6, yp), QString::number(axisLabel[i], 'f', 1));
            }
        }
    }
}

