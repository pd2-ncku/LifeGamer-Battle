#include "undeadsamurai.h"

UndeadSamurai::UndeadSamurai(int group, Battle *battle, QObject *parent)
    : Minion('7', "undead_samurai", 2500, 9, 0.1f, 400, 2, 60, group, battle, parent)
{
    if(group == 1) setTarget(2);
    else setTarget(1);
}
