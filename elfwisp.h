#ifndef ELFWISP_H
#define ELFWISP_H

#include "minion.h"
#include "battle.h"

class ElfWisp : public Minion
{
public:
    ElfWisp(int group, Battle *battle, QObject *parent);

    virtual void active();
};

#endif // ELFWISP_H
