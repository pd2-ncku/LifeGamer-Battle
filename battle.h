#ifndef BATTLE_H
#define BATTLE_H

#include <QObject>
#include <QTimer>
#include <QLinkedList>
#include <QProcess>

class Unit;

class Battle : public QObject
{
    Q_OBJECT
public:
    explicit Battle(QObject *parent = 0);
    ~Battle();
    void startBattle();
    void setCompetitor(QString path);

    friend class Minion;
    friend class Tower;
signals:

public slots:
    void readChildProcess();

private:
    char map[22][52];
    int countdown; /* 3 minutes countdown */
    QTimer *synchrogazer; /* for map synchronization(Listen to my song~) */
    QLinkedList<Unit*> UnitList;
    bool started;
    int mana_comp1, mana_comp2;

    QProcess *comp1;

    void initMap();
    void initTower(int SN);
    void destroyTower(int SN);

    int addMinion();

private slots:
    void clk(); /* Game clock */
};

#endif // BATTLE_H
