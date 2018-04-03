#ifndef ELFDANCER_H
#define ELFDANCER_H

#include "minion.h"
#include "battle.h"

class ElfDancer : public Minion
{
public:
    ElfDancer(int group, Battle *battle, QObject *parent);

    virtual void active();

    virtual ~ElfDancer();

private:
    int cnt;
};

#endif // ELFDANCER_H
