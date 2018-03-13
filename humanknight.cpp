#include "humanknight.h"

HumanKnight::HumanKnight(int group, Battle *battle, QObject *parent)
    : Minion('1', "human_knight", 750, 5, 0.3f, 150, 3, 21, group, battle, parent)
{
    if(group == 1) setTarget(2);
    else setTarget(1);
}
