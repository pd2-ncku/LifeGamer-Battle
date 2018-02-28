#include "sgram.h"

#include "tower.h"
#include <QtMath>

Sgram::Sgram(int group, Battle *battle, QObject *parent)
    : Minion('8', "sgram", 2000, 5, 0.1f, 1000, 2, 60, group, battle, parent)
{
    if(group == 1) setTarget(2);
    else setTarget(1);
}

void Sgram::active()
{
    Unit* Target = NULL;

    if(attackCnt) --attackCnt;
    /* search for nearest target */
    for(Unit* iter : battle->UnitList) {
        if(iter->group == target && (!Target || hypot(iter->x - fixed_x , iter->y - fixed_y) < hypot(Target->x - fixed_x , Target->y - fixed_y))){
            if(dynamic_cast<Tower*>(iter)) Target = iter;
        }
    }
    if(Target && hypot(Target->x - fixed_x , Target->y - fixed_y) < attackRange){
        if(!attackCnt) attackCnt = attackDelay;
        if((attackCnt < (attackDelay/2+5)) && (attackCnt > (attackDelay/2))){
            if(fixed_y - Target->y > 0) stat = StatusAttackLeft;
            else stat = StatusAttackRight;
        }
        else{
            if(fixed_y - Target->y > 0) stat = StatusStopLeft;
            else stat = StatusStopRight;
        }
        if(attackCnt == (attackDelay/2)) Target->onhit(atk);
    } else { /* not attacking */
        walk();
    }
}
