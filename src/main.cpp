#include "setuptool.h"
#include "settingsform.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    setuptool setuptool1;
    setuptool1.show();
    return app.exec();
}
