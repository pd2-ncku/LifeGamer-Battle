#ifndef TOWER_H
#define TOWER_H

#include "unit.h"
#include "battle.h"

class Tower : public Unit
{
public:
    Tower(int hp, int atk, int attackRange, int group, int target, Battle *battle, QObject *parent);

    void setPoint(int x, int y);
    int x;
    int y;
    int SN;

public slots:
    virtual void active();
};

#endif // TOWER_H
