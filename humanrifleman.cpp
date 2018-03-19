#include "humanrifleman.h"

HumanRifleman::HumanRifleman(int group, Battle *battle, QObject *parent)
    : Minion('C', "human_rifleman", 500, 7, 0.2f, 1000, 7, 50, group, battle, parent)
{
    if(group == 1) setTarget(2);
    else setTarget(1);
}
