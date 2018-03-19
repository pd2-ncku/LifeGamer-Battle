#include "humanthief.h"

HumanThief::HumanThief(int group, Battle *battle, QObject *parent)
    : Minion('3', "human_thief", 400, 2, 0.4f, 50, 3, 4, group, battle, parent)
{
    if(group == 1) setTarget(2);
    else setTarget(1);
}
