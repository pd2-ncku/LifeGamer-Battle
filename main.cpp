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
    QCoreApplication::setApplicationVersion("v1.0");

    QCommandLineParser parser;


    parser.addPositionalArgument("<p1>", QCoreApplication::translate("main", "Specific p1 program location."));
    parser.addPositionalArgument("<p2>", QCoreApplication::translate("main", "Specific p2 program location."));

    QCommandLineOption showMap(QStringList() << "m" << "map",
                QCoreApplication::translate("main", "Show map during game."));
    parser.addOption(showMap);

    QCommandLineOption echo(QStringList() << "e" << "echo",
                QCoreApplication::translate("main", "Echo your command."));
    parser.addOption(echo);

    QCommandLineOption p1_name(QStringList() << "1" << "p1",
                QCoreApplication::translate("main", "Set player 1 name"),
                QCoreApplication::translate("main", "player 1 name"));
    parser.addOption(p1_name);

    QCommandLineOption p2_name(QStringList() << "2" << "p2",
                QCoreApplication::translate("main", "Set player 2 name"),
                QCoreApplication::translate("main", "player 2 name"));
    parser.addOption(p2_name);

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
            cerr << "P1 start failed." << endl;
            return 0;
        }
        if(!battle.setP2(args.at(1))) {
            cerr << "P2 start failed." << endl;
            return 0;
        }
    }

    QString name1, name2;
    name1 = parser.value(p1_name);
    name2 = parser.value(p2_name);
    if(name1.isEmpty() || name2.isEmpty()) {
        cerr << parser.helpText().toStdString();
        return 0;
    }
    else {
        battle.setP1Name(name1);
        battle.setP2Name(name2);
    }

    return app.exec();
}
