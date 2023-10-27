#pragma once

#include "typedef.h"
#include <QVector>

class DataSrc
{
public:
    DataSrc();
    bool readFile(const QString &filename, QVector<QVector<d>> &fileArray);
    void readJson(const QString &filename, QVector<QVector<d>> &jsonArray);
    bool readAlpha(const QString &filename, QVector<QVector<d>> &alphaArray);
};
