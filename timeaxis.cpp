#include "timeaxis.h"
#include <QDateTime>

int getNumberOfDays(int month, int year)
{
    //leap year condition, if month is 2
    if( month == 2)
    {
        if((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
            return 29;
        else
            return 28;
    }
    //months which have 31 days
    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
        return 31;
    else
        return 30;
}


TimeAxis::TimeAxis() : timeAxisGap{8}
{
}

void TimeAxis::paint(QPainter &p, Origin &ori, Size &size, int &start, int &end, d &gap, QVector<QVector<d>> &data)
{
    p.drawLine(ori.x, ori.y, ori.x + size.w, ori.y); // horizontal axis line

    for (int i = 0; i < end - start; ++i)
    {
        QDateTime t = QDateTime::fromSecsSinceEpoch(data[i + start][time]);

        const int month = t.toString("M").toInt();
        const int year = t.toString("yyyy").toInt();
        const int day = t.toString("d").toInt();
        QString smon = t.toString("MMM");

        const int x = ori.x + i * gap;
        if (day == 1) {
            p.drawLine(x, ori.y, x, ori.y + 10);
            p.drawText(QPointF(x, ori.y + 30), smon);
        }
        else if (day % timeAxisGap == 0)
        {
            const int daysInMonth = getNumberOfDays(month, year);
            p.drawLine(x, ori.y, x, ori.y + 10);
            //if (day + (timeAxisGap - 1) <= daysInMonth && day - (timeAxisGap - 1) >= 1) // prevented ticks from being close to Oct, Sep text ticks. but use timeaxisgap 8 makes it good
            //{
            p.drawText(QPointF(x, ori.y + 30), QString::number(day));
            //}
        }
    }
}
