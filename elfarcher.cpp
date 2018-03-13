#include "elfarcher.h"

ElfArcher::ElfArcher(int group, Battle *battle, QObject *parent)
    : Minion('6', "elf_archer", 150, 4, 0.3f, 250, 5, 12, group, battle, parent)
{
    if(group == 1) setTarget(2);
    else setTarget(1);
}
