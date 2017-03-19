#ifndef HUMANTHIEF_H
#define HUMANTHIEF_H

#include "minion.h"
#include "battle.h"

class HumanThief : public Minion
{
public:
    HumanThief(int group, Battle *battle, QObject *parent);
};

#endif // HUMANTHIEF_H
