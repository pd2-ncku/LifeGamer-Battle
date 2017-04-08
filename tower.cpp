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
    bool attack = false;
    for(Unit* iter : battle->UnitList) {
        if(Minion* temp = dynamic_cast<Minion*>(iter)) {
            if(temp->group != group) {
                if(temp->fixed_y <= 23 && temp->group == 2) {
                    if((SN == 1 || SN == 2) && temp->fixed_x <= 10) {
                        attack = true;
                    }
                    else if((SN == 3 || SN == 2) && temp->fixed_x > 10) {
                        attack = true;
                    }
                }
                else if(temp->fixed_y >= 28 && temp->group == 1) {
                    if((SN == 4 || SN == 5) && temp->fixed_x <= 10) {
                        attack = true;
                    }
                    else if((SN == 6 || SN == 5) && temp->fixed_x > 10) {
                        attack = true;
                    }
                }

                if(attack) {
                    temp->onhit(atk);
                    break;
                }
            }
        }
    }
}
