#include <QtGui/QApplication>
#include "Paper Soccer.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Paper_Soccer paper soccer;
    paper soccer.show();
    return app.exec();
}
