#ifndef MINION_H
#define MINION_H

#include "unit.h"

class Minion : public Unit
{
public:
    Minion(char minion_num, QString type, int hp, int cost, float walkSpeed, int atk, int attackRange, int group, Battle *battle, QObject *parent);

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
    char minion_num;
    int self_number;
    static int number;
    QString type;

    virtual void walk();
    void setWalkDirection(float target_x, float target_y);
    void setAttackDirection(float target_x, float target_y);
};

#endif // MINION_H
