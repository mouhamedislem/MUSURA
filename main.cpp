#include <QApplication>
#include <QFontDatabase>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Les chemins correspondent à resources.qrc (préfixe /fonts)
    QFontDatabase::addApplicationFont(":/fonts/Cormorant_Garamond/CormorantGaramond-VariableFont_wght.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Montserrat/Montserrat-VariableFont_wght.ttf");

    MainWindow w;
    w.show();
    return app.exec();
}
