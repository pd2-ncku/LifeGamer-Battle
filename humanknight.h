#ifndef HUMANKNIGHT_H
#define HUMANKNIGHT_H

#include "minion.h"
#include "battle.h"

class HumanKnight : public Minion
{
public:
    HumanKnight(int group, Battle *battle, QObject *parent);
};

#endif // HUMANKNIGHT_H
