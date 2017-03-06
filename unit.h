#ifndef UNIT_H
#define UNIT_H

/* Base class of units(minions/towers) */

#include <QObject>

#include "battle.h"

class Unit : public QObject
{
    Q_OBJECT
public:
    explicit Unit(int hp,
                  int cost,
                  float walkSpeed,
                  int atk,
                  int attackSpeed,
                  int attackRange,
                  int effectRadius,
                  int group,
                  Battle *battle,
                  QObject *parent = 0);

    virtual ~Unit();

    enum status {
        StatusWalk,
        StatusAttack,
        StatusIdle,
        StatusPrepare
    };

    int getCurrentHp();

    int getMaxHp();

    int getHpRatio();

    int getCost();

    void onhit(int enemyATK);

    friend class Minion;
    friend class Tower;
    friend class Battle;

signals:
    void attack();

public slots:
    virtual void active(); /* Game clock */

private:
    const int MaxHp;
    int hp; /* Hitpoint */
    int cost; /* summon cost */
    float walkSpeed; /* walking speed */
    
    int atk; /* attack damage */
    int attackSpeed; /* attack cooldown(in 0.1 sec) */
    int attackRange; /* unit detect range */

    float aoeRadius; /* 0 when the unit is not AOE */
    int group; /* friend or enemy */

    Battle *battle; /* parent battle field */

};

#endif // UNIT_H
