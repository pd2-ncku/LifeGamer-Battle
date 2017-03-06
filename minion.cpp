#include "minion.h"

#include <QtMath>
#include <QDebug>

#include "tower.h"

Minion::Minion(int hp, int cost, float walkSpeed, int atk, int attackSpeed, int attackRange, int effectRadius, int group, Battle *battle, QObject *parent)
    : Unit(hp, cost, walkSpeed, atk, attackSpeed, attackRange, effectRadius, group, battle, parent)
{

}

void Minion::setPoint(int x, int y)
{
    this->x = x + 0.5f;
    this->y = y + 0.5f;
    fixed_x = static_cast<int>(this->x);
    fixed_y = static_cast<int>(this->y);
    battle->map[fixed_x][fixed_y] = 'M';
}

void Minion::active()
{
    bool inMySight = false;

    /* search for target */
    for(Unit* iter : battle->UnitList) {
        if(Tower* temp = dynamic_cast<Tower*>(iter)) {
            if(temp->group != group && (qPow(temp->x - fixed_x, 2) + qPow(temp->y - fixed_y, 2) < attackRange * attackRange)) {
                inMySight = true;
                temp->onhit(atk);
                break;
            }
        }
        else if(Minion* temp = dynamic_cast<Minion*>(iter)) {
            if(temp->group != group && (qPow(temp->x - fixed_x, 2) + qPow(temp->y - fixed_y, 2) < attackRange * attackRange)) {
                inMySight = true;
                temp->onhit(atk);
                break;
            }
        }
    }

    if(inMySight) { /* attack */

    }
    else walk();
}

void Minion::walk()
{
    int target_x, target_y;

    /* decide walking target */
    if(group == 1) { /* left side */
        if(x <= 10) {
            if(y < 25) { /* upper bridge */
                target_x = 7;
                target_y = 25;
            }
            else if(battle->map[6][41] == '#') { /* upper tower */
                target_x = 6;
                target_y = 41;
            }
            else { /* lower tower */
                target_x = 11;
                target_y = 43;
            }
        }
        else {
            if(y < 25) { /* upper bridge */
                target_x = 14;
                target_y = 25;
            }
            else if(battle->map[15][41] == '#') { /* upper tower */
                target_x = 15;
                target_y = 41;
            }
            else {
                target_x = 11;
                target_y = 43;
            }
        }
    }
    else { /* group2 */
        if(x <= 10) {
            if(y > 26) { /* upper bridge */
                target_x = 7;
                target_y = 26;
            }
            else if(battle->map[6][10] == '#') { /* upper tower */
                target_x = 6;
                target_y = 10;
            }
            else {
                target_x = 10;
                target_y = 8;
            }
        }
        else {
            if(y > 26) { /* upper bridge */
                target_x = 14;
                target_y = 26;
            }
            else if(battle->map[15][10] == '#') { /* upper tower */
                target_x = 15;
                target_y = 10;
            }
            else {
                target_x = 10;
                target_y = 8;
            }
        }
    }

    float base = qFabs(target_x - x) + qFabs(target_y - y);
    int temp_x = static_cast<int>(x);
    int temp_y = static_cast<int>(y);

    if(fixed_x == temp_x) x += (target_x - x) * walkSpeed / base;
    if(fixed_y == temp_y) y += (target_y - y) * walkSpeed / base;

    battle->map[fixed_x][fixed_y] = ' ';

    if(fixed_x != static_cast<int>(x)) {
        if(battle->map[temp_x][fixed_y] == ' ') {
            fixed_x = temp_x;
        }
    }
    if(fixed_y != static_cast<int>(y)) {
        if(battle->map[fixed_x][temp_y] == ' ') {
            fixed_y = temp_y;
        }
    }

    battle->map[fixed_x][fixed_y] = 'M';
}
