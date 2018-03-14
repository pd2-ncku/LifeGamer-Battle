#ifndef BATTLE_H
#define BATTLE_H

#include <QObject>
#include <QTimer>
#include <QLinkedList>
#include <QProcess>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QString>

#include "player.h"
#include "rendercommunicator.h"
#include "judgecommunicator.h"

/* Bash color code */
#define NONE "\033[m"
#define RED "\033[1;32;31m"
#define GREEN "\033[1;32;32m"
#define BLUE "\033[1;32;34m"
#define CYAN "\033[1;36m"
#define PURPLE "\033[1;35m"
#define YELLOW "\033[1;33m"
#define WHITE "\033[1;37m"

class Unit;

class Battle : public QObject
{
    Q_OBJECT
public:
    explicit Battle(QObject *parent = 0);
    ~Battle();
    void startBattle();
    bool setP1(QString path);
    bool setP2(QString path);
    void setMapOutput();
    void setEchoOutput();
    void setP1Name(QString name);
    void setP2Name(QString name);

    enum summon_result {
        SummonSuccess,
        SummonFailedOM,
        SummonFailedOccupied,
        SummonFailedUnknowMinion,
        SummonFailedNotInDeck,
        SummonFailedOutOfRange
    };

    friend class Minion;
    friend class Tower;

    friend class ElfArcher;
    friend class ElfGiant;
    friend class ElfWisp;
    friend class ElfDancer;
    friend class HumanKnight;
    friend class HumanPriest;
    friend class HumanThief;
    friend class Sgram;
    friend class UndeadSamurai;
    // Add rifleman
    friend class HumanRifleman;

signals:
    void signalLogHp();
    void endGame();
    void decideWinLose(int SN);
    void playerReady(int player);
    void quit();

public slots:
    void p1_error(QProcess::ProcessError error);
    void p2_error(QProcess::ProcessError error);

private:
    char map[22][53];
    char map_hp[22][53];
    int countdown; /* 3 minutes countdown */
    QTimer *synchrogazer; /* for map synchronization(Listen to my song~) */
    QLinkedList<Unit*> UnitList;
    bool started;
    RenderCommunicator* render;
    JudgeCommunicator* judge;

    bool displayMap;
    bool echoCommand;
    bool judged;

    Player *p1;
    Player *p2;

    int minion_cost[24];

    void initMap();
    void initTower(int SN);
    void destroyTower(int SN);
    void modifyTowerHp(int SN, int hpRatio);

    int addMinion(int player, int num, int x, int y);

private slots:
    void clk(); /* Game clock */
    void gameFinished(int SN);
    void postSolve();
};

#endif // BATTLE_H
