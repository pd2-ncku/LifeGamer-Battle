#ifndef ELFARCHER_H
#define ELFARCHER_H

#include "minion.h"
#include "battle.h"

class ElfArcher : public Minion
{
public:
    ElfArcher(int group, Battle *battle, QObject *parent);
};

#endif // ELFARCHER_H
