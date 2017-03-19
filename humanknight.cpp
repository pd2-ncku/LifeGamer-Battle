#include "humanknight.h"

HumanKnight::HumanKnight(int group, Battle *battle, QObject *parent)
    : Minion('1', "human_knight", 1500, 5, 0.3f, 5, 3, group, battle, parent)
{
    if(group == 1) setTarget(2);
    else setTarget(1);
}
