#include "elfgiant.h"

ElfGiant::ElfGiant(int group, Battle *battle, QObject *parent)
    : Minion('4', "elf_giant", 3500, 7, 0.2f, 1, 2, group, battle, parent)
{
    if(group == 1) setTarget(2);
    else setTarget(1);
}
