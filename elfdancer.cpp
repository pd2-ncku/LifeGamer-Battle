#include "elfdancer.h"

ElfDancer::ElfDancer(int group, Battle *battle, QObject *parent)
    : Minion('9', "elf_dancer", 1000, 5, 0.3f, 0, 0, group, battle, parent)
{
    cnt = 0;
}

void ElfDancer::active()
{
    cnt++;
    if(!(cnt % 10)) {
        if(group == 1) battle->p1->addMana(1);
        else battle->p2->addMana(1);
        hp -= 100;
    }

    walk();
}
