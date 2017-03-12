#include "unit.h"

Unit::Unit(int hp, int cost, float walkSpeed, int atk, int attackRange, int group, int target, Battle *battle, QObject *parent)
    : QObject(parent),
      MaxHp(hp),
      hp(hp),
      cost(cost),
      walkSpeed(walkSpeed),
      atk(atk),
      attackRange(attackRange),
      group(group),
      target(target),
      battle(battle)
{

}

Unit::~Unit()
{

}

int Unit::getCurrentHp()
{
    return hp;
}

int Unit::getMaxHp()
{
    return MaxHp;
}

int Unit::getHpRatio()
{
    return (hp * 100) / MaxHp;
}

int Unit::getHpChange()
{
    return getHpRatio() - previousHpRatio;
}

int Unit::getCost()
{
    return cost;
}

void Unit::onhit(int enemyATK)
{
    hp -= enemyATK;

    /* friend healer will not to overheal */
    if(hp > MaxHp) hp = MaxHp;
}

void Unit::active()
{

}

void Unit::setPreviousHpRatio()
{
    previousHpRatio = getHpRatio();
}
