#include "tower.h"

#include "minion.h"

#include <QtMath>
#include <QDebug>

Tower::Tower(int hp, int atk, int attackRange, int attackDelay, int group, int target, Battle *battle, QObject *parent)
    : Unit(hp, 0, 0, atk, attackRange, attackDelay, group, battle, parent)
{
    connect(battle, SIGNAL(signalLogHp()), this, SLOT(setPreviousHpRatio()));
    if(group == 1) setTarget(2);
    else setTarget(1);
}

void Tower::setPoint(int x, int y)
{
    this->x = x;
    this->y = y;
}

QJsonObject Tower::toJsonObject(bool isNew)
{
    QJsonObject tower;
    QString name("p");
    name += QString::number(group);
    name += "_";
    switch(SN) {
    case 1:case 4:
        name += "top";
        break;
    case 2:case 5:
        name += "main";
        break;
    case 3: case 6:
        name += "down";
        break;
    }

    tower["name"] = name;
    tower["status"] = QString::number(getHpChange());

    return tower;
}

void Tower::active()
{
    Minion* Target = NULL;

    if(attackCnt) --attackCnt;
    /* search for nearest target */
    for(Unit* iter : battle->UnitList) {
        if(Minion* temp = dynamic_cast<Minion*>(iter)) {
            if(temp->group == target &&
                (Target == NULL || hypot(temp->x - x , temp->y - y) < hypot(Target->x - x , Target->y - y)))
            {
                Target = temp;
            }
        }
    }
    if(Target && hypot(Target->x - x , Target->y - y) < attackRange){
        if(!attackCnt) attackCnt = attackDelay;
        if(attackCnt == (attackDelay/2)) Target->onhit(atk);
    }
}
