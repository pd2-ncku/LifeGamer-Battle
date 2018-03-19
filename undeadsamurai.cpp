#include "undeadsamurai.h"

UndeadSamurai::UndeadSamurai(int group, Battle *battle, QObject *parent)
    : Minion('7', "undead_samurai", 3000, 8, 0.2f, 500, 4, 20, group, battle, parent)
{
    if(group == 1) setTarget(2);
    else setTarget(1);
}
