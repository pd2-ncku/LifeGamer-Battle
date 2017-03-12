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
                  int attackRange,
                  int group, int target,
                  Battle *battle,
                  QObject *parent = 0);

    virtual ~Unit();

    enum status {
        StatusWalkLeft = 0,
        StatusWalkRight,
        StatusWalkTop,
        StatusWalkDown,
        StatusWalkLT,
        StatusWalkLD,
        StatusWalkRT,
        StatusWalkRD,
        StatusAttackLeft,
        StatusAttackRight,
        StatusStopRight,
        StatusStopLeft
    };

    int getCurrentHp();

    int getMaxHp();

    int getHpRatio();

    int getHpChange();

    int getCost();

    void onhit(int enemyATK);

    friend class Minion;
    friend class Tower;
    friend class Battle;

public slots:
    virtual void active(); /* Game clock */
    void setPreviousHpRatio();

private:
    const int MaxHp;
    int hp; /* Hitpoint */
    int cost; /* summon cost */
    float walkSpeed; /* walking speed */
    
    int atk; /* attack damage */
    int attackRange; /* unit detect range */

    int group; /* friend or enemy */
    int target; /* healers use minus attack at friend */

    Battle *battle; /* parent battle field */

    int stat;
    int previousHpRatio;

};

#endif // UNIT_H
