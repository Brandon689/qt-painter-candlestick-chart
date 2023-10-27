#include "ward.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MWidget w;
    w.show();

    return app.exec();
}
