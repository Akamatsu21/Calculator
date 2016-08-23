#include "maincalculatorwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainCalculatorWindow calc;
    calc.show();

    return app.exec();
}
