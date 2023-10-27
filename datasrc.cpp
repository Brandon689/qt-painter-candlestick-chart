#include "datasrc.h"

#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDate>
#include <QDebug>

DataSrc::DataSrc()
{
}

bool DataSrc::readFile(const QString &filename, QVector<QVector<double>> &fileArray)
{
    QFile acme(filename);
    acme.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&acme);

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (line.startsWith("#") || line.isEmpty())
            continue;
        QStringList strList = line.split(" ", Qt::SkipEmptyParts);

        cd timestamp = strList.at(0).toDouble();
        cd open = strList.at(1).toDouble();
        cd high = strList.at(2).toDouble();
        cd low = strList.at(3).toDouble();
        cd close = strList.at(4).toDouble();
        fileArray.append({timestamp, open, high, low, close});
    }
    acme.close();
    return 0;
}

bool DataSrc::readAlpha(const QString &filename, QVector<QVector<double>> &alphaArray)
{
//    QFile alpha(filename);
//    alpha.open(QIODevice::ReadOnly | QIODevice::Text);
//    QTextStream stream(&alpha);

//    while (!stream.atEnd()) {
//        QString line = stream.readLine();
//        if (line.startsWith("timestamp") || line.isEmpty())
//            continue;
//        QStringList strList = line.split(",", QString::SkipEmptyParts);

//        cd date = strList.at(0).toDouble(); // not double, handle yyyy-mm-d type dates
//        cd open = strList.at(1).toDouble();
//        cd high = strList.at(2).toDouble();
//        cd low = strList.at(3).toDouble();
//        cd close = strList.at(4).toDouble();
//        cd volume = strList.at(5).toDouble();
//        alphaArray.append({date, open, high, low, close, volume});
//    }
//    alpha.close();
//    return 0;

    QFile alpha(filename);
    alpha.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream stream(&alpha);

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (line.startsWith("Date") || line.isEmpty())
            continue;
        QStringList strList = line.split(",", Qt::SkipEmptyParts);


        QStringList dateList = strList.at(0).split("-");
        int yr = dateList.at(0).toInt();
        int mn = dateList.at(1).toInt();
        int dy = dateList.at(2).toInt();
        QDate qdate(yr, mn, dy);
        QDateTime qtime(qdate, QTime(0, 0));

        cd date = qtime.toSecsSinceEpoch(); // not double, handle yyyy-mm-d type dates
        cd open = strList.at(1).toDouble();
        cd high = strList.at(2).toDouble();
        cd low = strList.at(3).toDouble();
        cd close = strList.at(4).toDouble();
        cd volume = strList.at(5).toDouble();
        alphaArray.append({date, open, high, low, close, volume});
    }
    alpha.close();
    return 0;
}

void DataSrc::readJson(const QString &filename, QVector<QVector<double>> &jsonArray)
{
    QFile f(filename);
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray json = f.readAll();
    f.close();
    QJsonDocument document = QJsonDocument::fromJson(json);
    QJsonArray fdata = document.array();
    if (fdata.isEmpty()) { qWarning() << "The array is empty"; }

    for (auto d : fdata)
    {
        QJsonArray arr = d.toArray();
        cd time = arr.at(0).toDouble() / 1000;
        cd open = arr.at(1).toVariant().toDouble();
        cd high = arr.at(2).toVariant().toDouble();
        cd low = arr.at(3).toVariant().toDouble();
        cd close = arr.at(4).toVariant().toDouble();

        jsonArray.append({time, open, high, low, close});
    }
}
