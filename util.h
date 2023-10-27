#pragma once

#include "typedef.h"
#include <QVector>
#include <QFile>
#include <QJsonDocument>
#include <cmath>

// point on Y-Axis

inline double toCoord(cd &val, cd &height, cd &max, cd &min)
{
    return height - (val - min) * (height / (max - min));
}

inline double toLogCoord(cd &val, cd &height, cd &max, cd &min)
{
    double slmin = std::log10(min);
    double slmax = std::log10(max);
    double normalize = (std::log10(val) - slmin) / (slmax - slmin);

    double ret = height - (height * normalize); // h - to flip it, times log bit to get it to coord
    return ret;


//    double last = std::log10(max);
//    double zoo = std::log10(min) / last; // / last
//    double range = 1 - zoo;
//    double s;
//    double g;
//    s = std::log10(val) / last;

//    g = 1 - s;
//    double rat = height / range;
//    double cg = g * rat;
//    //p.drawText(QPointF(105, h - cg + 16), QString::number(u, 'f', 3));

//    return cg;



//    d mi = min;//std::log10(min);
//    d ma = max;//std::log10(max);
//    d va = val;//std::log10(val);
//    d coord = 1; // top, be zero
//    d pl = height;
//    d top = max;

//    if (top == 0)
//        top = 0.000001;
//    if (va == 0)
//        va =  0.000001;
//    //log base 2 (5) = (log base 10 (5) ) / (log base 10 (2) )

//    cd log = std::log10(va) / std::log10(max - min);

//    cd ret = pl * log;

//    return ret;
    //cd scale = height / (ma - mi);
    //return height - (va - mi) * scale;
}

inline double toValue(cd &coord, cd &height, cd &max, cd &min)
{
    return coord / (height / (max - min));
}
