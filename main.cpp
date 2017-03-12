#include <QCoreApplication>
#include <QDebug>

#include "battle.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Battle battle;
    QObject::connect(&battle, SIGNAL(endGame()), &app, SLOT(quit()));
    if(argc == 2) {
        battle.setCompetitor(argv[1]);
    }
    else {
        qDebug() << "Usage: ";
        return 0;
    }

    return app.exec();
}
