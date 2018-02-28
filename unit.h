#ifndef UNIT_H
#define UNIT_H

/* Base class of units(minions/towers) */

#include <QObject>
#include <QJsonObject>

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
                  int attackDelay,
                  int group,
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

    void setTarget(int target);

    void onhit(int enemyATK);

    virtual QJsonObject toJsonObject(bool isNew = false) = 0;

    friend class Minion;
    friend class Tower;
    friend class Battle;

    friend class ElfArcher;
    friend class ElfGiant;
    friend class ElfWisp;
    friend class ElfDancer;
    friend class HumanKnight;
    friend class HumanPriest;
    friend class HumanThief;
    friend class Sgram;
    friend class UndeadSamurai;
    // Add Rifleman
    friend class HumanRifleman;

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
    int attackDelay; /* unit attack delay */
    int attackCnt = 0; /*unit attack delay count */

    float x;
    float y;

    int group; /* friend or enemy */
    int target; /* healers use minus attack at friend */

    Battle *battle; /* parent battle field */

    int stat;
    int previousHpRatio;

};

#endif // UNIT_H
