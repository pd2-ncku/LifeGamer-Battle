#include "unit.h"



Unit::Unit(int hp, int cost, float walkSpeed, int atk, int attackSpeed, int attackRange, int effectRadius, int group, Battle *battle, QObject *parent)
    : QObject(parent),
      MaxHp(hp),
      hp(hp),
      cost(cost),
      walkSpeed(walkSpeed),
      atk(atk),
      attackSpeed(attackSpeed),
      attackRange(attackRange),
      aoeRadius(effectRadius),
      group(group),
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
