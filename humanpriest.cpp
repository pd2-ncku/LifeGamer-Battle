#include "humanpriest.h"

#include <QtMath>

HumanPriest::HumanPriest(int group, Battle *battle, QObject *parent)
    : Minion('2', "human_priest", 350, 3, 0.2f, -50, 5, 45, group, battle, parent)
{
    setTarget(group);
}

