#ifndef BATTLE_H
#define BATTLE_H

#include <QObject>
#include <QTimer>
#include <QLinkedList>
#include <QProcess>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

class Unit;

class Battle : public QObject
{
    Q_OBJECT
public:
    explicit Battle(QObject *parent = 0);
    ~Battle();
    void startBattle();
    void setCompetitor(QString path);

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
signals:
    void signalLogHp();
    void endGame();
    void decideWinLose(int SN);

public slots:
    void readChildProcess();

private:
    char map[22][53];
    char map_hp[22][53];
    int countdown; /* 3 minutes countdown */
    QTimer *synchrogazer; /* for map synchronization(Listen to my song~) */
    QLinkedList<Unit*> UnitList;
    bool started;
    int mana_comp1;
    QNetworkAccessManager* serverConnection;

    QProcess *comp1;
    QString comp1_command;
    int deck[4];
    int todraw[4];
    int minion_cost[8];

    void initMap();
    void initTower(int SN);
    void destroyTower(int SN);
    void modifyTowerHp(int SN, int hpRatio);

    int addMinion(int num, int x, int y);

private slots:
    void clk(); /* Game clock */
    void childStarted();
    void gameFinished(int SN);
};

#endif // BATTLE_H
