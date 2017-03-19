#ifndef ELFGIANT_H
#define ELFGIANT_H

#include "minion.h"
#include "battle.h"

class ElfGiant : public Minion
{
public:
    ElfGiant(int group, Battle *battle, QObject *parent);
};

#endif // ELFGIANT_H
