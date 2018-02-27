#include "sgram.h"

#include "tower.h"
#include <QtMath>

Sgram::Sgram(int group, Battle *battle, QObject *parent)
    : Minion('8', "sgram", 1500, 5, 0.1f, 750, 2, 90, group, battle, parent)
{
    if(group == 1) setTarget(2);
    else setTarget(1);
}

void Sgram::active()
{
    bool inMySight = false;

    /* search for target */
    for(Unit* iter : battle->UnitList) {
        if(Tower* temp = dynamic_cast<Tower*>(iter)) {
            if(temp->group == target && (qPow(temp->x - fixed_x, 2) + qPow(temp->y - fixed_y, 2) < attackRange * attackRange)) {
                inMySight = true;
                if(fixed_y - temp->y > 0) stat = StatusAttackLeft;
                else stat = StatusAttackRight;
                temp->onhit(atk);
                break;
            }
        }
    }

    if(!inMySight) { /* not attacking */
        walk();
    }
}
