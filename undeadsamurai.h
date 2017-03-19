#ifndef UNDEADSAMURAI_H
#define UNDEADSAMURAI_H

#include "minion.h"
#include "battle.h"

class UndeadSamurai : public Minion
{
public:
    UndeadSamurai(int group, Battle *battle, QObject *parent);
};

#endif // UNDEADSAMURAI_H
