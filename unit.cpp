#include "unit.h"

Unit::Unit(int hp, int cost, float walkSpeed, int atk, int attackRange, int attackDelay, int group, Battle *battle, QObject *parent)
    : QObject(parent),
      MaxHp(hp),
      hp(hp),
      cost(cost),
      walkSpeed(walkSpeed),
      atk(atk),
      attackRange(attackRange),
      attackDelay(attackDelay),
      group(group),
      battle(battle)
{
    previousHpRatio = 100;
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
    if(hp <= 0) return -100;
    else return getHpRatio() - previousHpRatio;
}

int Unit::getCost()
{
    return cost;
}

void Unit::setTarget(int target)
{
    this->target = target;
}

void Unit::onhit(int enemyATK)
{
    hp -= enemyATK;

    /* friend healer will not to overheal */
    if(hp >= MaxHp) hp = MaxHp;
}

void Unit::active()
{

}

void Unit::setPreviousHpRatio()
{
    previousHpRatio = getHpRatio();
}
