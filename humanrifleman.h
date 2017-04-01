#ifndef HUMANRIFLEMAN_H
#define HUMANRIFLEMAN_H

#include "minion.h"
#include "battle.h"

class HumanRifleman : public Minion
{
public:
    HumanRifleman(int group, Battle *battle, QObject *parent);
};

#endif // HUMANRIFLEMAN_H
