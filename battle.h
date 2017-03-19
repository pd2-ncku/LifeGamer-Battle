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
    bool setP1(QString path);
    bool setP2(QString path);
    void setMapOutput();
    void setEchoOutput();

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
    void playerReady(int player);
    void quit();

public slots:
    void readP1();
    void readP2();
    void p1_error(QProcess::ProcessError error);
    void p2_error(QProcess::ProcessError error);

private:
    char map[22][53];
    char map_hp[22][53];
    int countdown; /* 3 minutes countdown */
    QTimer *synchrogazer; /* for map synchronization(Listen to my song~) */
    QLinkedList<Unit*> UnitList;
    bool started;
    int p1_mana;
    int p2_mana;
    QNetworkAccessManager* serverConnection;
    bool displayMap;
    bool echoCommand;
    bool judged;

    QProcess *p1;
    QString p1_cmd;
    int p1_deck[4];
    int p1_todraw[4];
    bool p1_ready;

    QProcess *p2;
    QString p2_cmd;
    int p2_deck[4];
    int p2_todraw[4];
    bool p2_ready;

    int minion_cost[8];

    void initMap();
    void initTower(int SN);
    void destroyTower(int SN);
    void modifyTowerHp(int SN, int hpRatio);

    int addMinion(int player, int num, int x, int y);

    void p1_reg();
    void p2_reg();

private slots:
    void clk(); /* Game clock */
    void changePlayerState(int player);
    void gameFinished(int SN);
    void postSolve();
};

#endif // BATTLE_H
