#include <QCoreApplication>
#include <QCommandLineParser>
#include <QStringList>
#include <QDebug>

#include <iostream>
using namespace std;

#include "battle.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("PD2 proj.1 debug tool");

    QCommandLineParser parser;

    parser.addHelpOption();
    parser.addPositionalArgument("path", QCoreApplication::translate("main", "Specific your program location."));

    QCommandLineOption showMap(QStringList() << "m" << "map",
                QCoreApplication::translate("main", "Show map during game."));
    parser.addOption(showMap);

    parser.process(app);

    const QStringList args = parser.positionalArguments();

    Battle battle;
    QObject::connect(&battle, SIGNAL(endGame()), &app, SLOT(quit()));

    if(!args.size()) {
        cerr << parser.helpText().toStdString();
        return 0;
    }
    else {
        if(parser.isSet(showMap)) {
            battle.setMapOutput();
        }
        battle.setCompetitor(args.at(0));
    }

    return app.exec();
}
