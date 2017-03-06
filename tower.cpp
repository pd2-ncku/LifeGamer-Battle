#include "tower.h"

#include "minion.h"

Tower::Tower(int hp, int atk, int attackSpeed, int attackRange, int aoeRadius, int group, Battle *battle, QObject *parent)
    : Unit(hp, 0, 0, atk, attackSpeed, attackRange, aoeRadius, group, battle, parent)
{

}

void Tower::setPoint(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Tower::active()
{
    for(Unit* iter : battle->UnitList) {
        if(Minion* temp = dynamic_cast<Minion*>(iter)) {
            if(temp->group != group) {
                temp->onhit(atk);
                break;
            }
        }
    }
}
