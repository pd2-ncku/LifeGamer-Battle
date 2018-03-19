#include "humanpriest.h"

#include <QtMath>

HumanPriest::HumanPriest(int group, Battle *battle, QObject *parent)
    : Minion('2', "human_priest", 600, 3, 0.2f, -150, 5, 30, group, battle, parent)
{
    setTarget(group);
}

