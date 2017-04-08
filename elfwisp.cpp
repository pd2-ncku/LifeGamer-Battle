#include "elfwisp.h"

#include <QtMath>

ElfWisp::ElfWisp(int group, Battle *battle, QObject *parent)
    : Minion('5', "elf_wisp", 300, 1, 0.3f, -1, 4, group, battle, parent)
{
    setTarget(group);
}

void ElfWisp::active()
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
            }
        }
    }

    if(!inMySight) { /* not attacking */
        walk();
    }
}
