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
    QCoreApplication::setApplicationName("PD2 proj.1 debugtool");
    QCoreApplication::setApplicationVersion("v2.1");

    QCommandLineParser parser;


    parser.addPositionalArgument("<p1>", QCoreApplication::translate("main", "Specific p1 program location."));
    parser.addPositionalArgument("<p2>", QCoreApplication::translate("main", "Specific p2 program location."));

    QCommandLineOption showMap(QStringList() << "m" << "map",
                QCoreApplication::translate("main", "Show map during game."));
    parser.addOption(showMap);

    QCommandLineOption echo(QStringList() << "e" << "echo",
                QCoreApplication::translate("main", "Echo your command."));
    parser.addOption(echo);

    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(app);

    const QStringList args = parser.positionalArguments();

    Battle battle;
    QObject::connect(&battle, SIGNAL(quit()), &app, SLOT(quit()));

    if(args.size() < 2) {
        cerr << parser.helpText().toStdString();
        return 0;
    }
    else {
        if(parser.isSet(showMap)) {
            battle.setMapOutput();
        }
        if(parser.isSet(echo)) {
            battle.setEchoOutput();
        }
        if(!battle.setP1(args.at(0))) {
            return 0;
        }
        if(!battle.setP2(args.at(1))) {
            return 0;
        }
    }

    return app.exec();
}
