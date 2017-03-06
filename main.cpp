#include <QCoreApplication>
#include <QDebug>

#include "battle.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "main start";
    Battle battle;
    battle.startBattle();

    return app.exec();
}
