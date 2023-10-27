#pragma once

#include <cmath>

double niceNumber(double x, bool ceiling)
{
    double z = pow(10, floor(std::log10(x))); //find corresponding number of the form of 10^n than is smaller than x
    double q = x / z; //q<10 && q>=1;

    if (ceiling) {
        if (q <= 1.0) q = 1;
        else if (q <= 2.0) q = 2;
        else if (q <= 5.0) q = 5;
        else q = 10;
    } else {
        if (q < 1.5) q = 1;
        else if (q < 3.0) q = 2;
        else if (q < 7.0) q = 5;
        else q = 10;
    }
    return q * z;
}

void looseNiceNumbers(double &min, double &max, int &ticksCount)
{
    double range = niceNumber(max - min, true); //range with ceiling
    double step = niceNumber(range / (ticksCount - 1), false);
    min = floor(min / step);
    max = ceil(max / step);
    ticksCount = int(max - min) + 1;
    min *= step;
    max *= step;
    //qDebug() << "max:" << max << range << endl;
}
