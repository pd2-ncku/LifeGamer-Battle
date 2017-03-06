#ifndef MINION_H
#define MINION_H

#include "unit.h"

class Minion : public Unit
{
public:
    Minion(int hp, int cost, float walkSpeed, int atk, int attackSpeed, int attackRange, int effectRadius, int group, Battle *battle, QObject *parent);

    void setPoint(int x, int y);

    friend class Battle;
    friend class Tower;

public slots:
    virtual void active();

private:
    float x;
    float y;
    int fixed_x;
    int fixed_y;

    void walk();
};

#endif // MINION_H
