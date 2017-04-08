#include "humanpriest.h"

#include <QtMath>

HumanPriest::HumanPriest(int group, Battle *battle, QObject *parent)
    : Minion('2', "human_priest", 700, 3, 0.2f, -5, 5, group, battle, parent)
{
    setTarget(group);
}

void HumanPriest::active()
{
    bool inMySight = false;

    /* search for target */
    for(Unit* iter : battle->UnitList) {
        if(Minion* temp = dynamic_cast<Minion*>(iter)) {
            if(temp->self_number != self_number
                    && temp->group == target
                    && (qPow(temp->x - fixed_x, 2) + qPow(temp->y - fixed_y, 2) < attackRange * attackRange)
                    && temp->hp < temp->MaxHp) {
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


