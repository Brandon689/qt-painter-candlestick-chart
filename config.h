#pragma once

#include <QObject>
#include <QColor>
#include <QVector>

struct Theme
{
    QColor bg;
    QColor axis;
    QColor up;
    QColor down;
    QColor upOutline;
    QColor downOutline;
    QColor upWick;
    QColor downWick;
    QColor lineSeries;
};

class Config : public QObject
{
    Q_OBJECT
public:
    Config() : candleW(10), minPoints(8)
    {
        type = CANDLE;
        outline = true;
        xmargin = 66;
        ymargin = 50;
        autoRange = false;
        autoAlignPointCenter = true;
        side = Right;
        // theme colours
        t.lineSeries = QColor(0, 0, 0);
        t.upOutline = QColor(0, 0, 0);
        t.downOutline = QColor(0, 0, 0);
    }
protected:
    QString theme;

    Theme t;

    bool outline;
    const int candleW;
    bool autoRange; // fixed vertical axis auto
    int xmargin;
    int ymargin;
    int minPoints;

    enum AxisSide { Left, Right };
    AxisSide side;
    bool autoAlignPointCenter; // obscure (auto align left point to center position when block movements such as key scroll

    enum SeriesType { LINE, CANDLE };
    SeriesType type;

    friend class Chart;
    friend class CWidget;
    friend class Dialog;

public slots:
    void themeChanged()
    {
        if (theme == "Light") {
            t.bg = QColor(255,255,255);
            t.axis = QColor(0, 0, 0);
            t.up = QColor(239, 83, 80);
            t.down = QColor(38,166,154);
            t.downWick = t.up;
            t.upWick = t.down;
        }
        else if (theme == "Sand") {
            t.bg = QColor(243,236,224); // ori
            //t.bg = QColor(242,236,227);
            t.axis = QColor(0, 0, 0);
            //t.up = QColor(190,169,134); // ori
            t.up = QColor(183,163,130);
            t.down = QColor(130,112,83);
        }
        else if (theme == "Dark") {
            t.bg = QColor(19,23,34);
            t.axis = QColor(255, 255, 255);
            t.up = QColor(239, 83, 80);
            t.down = QColor(38,166,154);
            t.downWick = t.up;
            t.upWick = t.down;
        }
//        else if (theme == "Night") {
//            bgColor = QColor(26,20,39);
//            axisColor = QColor(255, 255, 255);
//            up = bgColor;
//            down = QColor(70,38,100);
//            outlineColor = QColor(220,255,230);
//            outline = true;
//        }
//        else if (theme == "Meta Green") {
//            bgColor = QColor(0, 0, 0);
//            axisColor = QColor(255, 255, 255);
//            up = bgColor;
//            down = QColor(255,255,255);
//            outline = true;
//            outlineColor = QColor(0, 255, 0);
//        }
//        else if (theme == "Icy") {
//            bgColor = QColor(255, 255, 255);
//            axisColor = QColor(0, 0, 0);
//            up = QColor(146, 210, 234);
//            down = QColor(55, 156, 196);
//            outline = true;
//            outlineColor = QColor(12, 38, 45);
//        }
//        else if (theme == "Nature") {
//            bgColor = QColor(255, 255, 255);
//            axisColor = QColor(0, 0, 0);
//            up = QColor(128, 195, 66);
//            down = QColor(0, 127, 42);
//            outline = false;
//        }
//        else if (theme == "Batman") {
//            bgColor = QColor(255, 255, 255);
//            axisColor = QColor(0, 0, 0);
//            up = QColor(255, 233, 0);
//            down = QColor(0, 0, 0);
//            outline = false;
//        }
    }
};
