#include "humanthief.h"

HumanThief::HumanThief(int group, Battle *battle, QObject *parent)
    : Minion('3', "human_thief", 500, 4, 0.4f, 125, 3, 9, group, battle, parent)
{
    if(group == 1) setTarget(2);
    else setTarget(1);
}
