#include "elfdancer.h"

ElfDancer::ElfDancer(int group, Battle *battle, QObject *parent)
    : Minion('9', "elf_dancer", 300, 3, 0.2f, 0, 0, 20, group, battle, parent)
{
    cnt = 0;
}

void ElfDancer::active()//will move this cnt to minion::active
{
    attackCnt++;
    if(attackCnt==attackDelay) {
        if(group == 1) battle->p1->addMana(1);
        else battle->p2->addMana(1);
        hp -= 60;
        attackCnt = 0;
    }

    walk();
}
