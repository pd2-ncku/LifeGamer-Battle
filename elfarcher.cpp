#include "elfarcher.h"

ElfArcher::ElfArcher(int group, Battle *battle, QObject *parent)
    : Minion('6', "elf_archer", 700, 4, 0.3f, 180, 5, 10, group, battle, parent)
{
    if(group == 1) setTarget(2);
    else setTarget(1);
}
