#include "sgram.h"

Sgram::Sgram(int group, Battle *battle, QObject *parent)
    : Minion('8', "sgram", 1500, 5, 0.1f, 20, 2, group, battle, parent)
{
    if(group == 1) setTarget(2);
    else setTarget(1);
}
