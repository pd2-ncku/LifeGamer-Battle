#include "tower.h"

#include "minion.h"

#include <QDebug>

Tower::Tower(int hp, int atk, int attackRange, int group, int target, Battle *battle, QObject *parent)
    : Unit(hp, 0, 0, atk, attackRange, group, battle, parent)
{
    connect(battle, SIGNAL(signalLogHp()), this, SLOT(setPreviousHpRatio()));
    this->target = target;
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
            if(temp->group != group && ((temp->fixed_y <= 23 && temp->group == 2) || (temp->fixed_y >= 28 && temp->group == 1))) {
                temp->onhit(atk);
                break;
            }
        }
    }
}
