#include "humanpriest.h"

HumanPriest::HumanPriest(int group, Battle *battle, QObject *parent)
    : Minion('2', "human_priest", 700, 3, 0.2f, -5, 5, group, battle, parent)
{
    setTarget(group);
}
