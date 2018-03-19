#include "elfgiant.h"

ElfGiant::ElfGiant(int group, Battle *battle, QObject *parent)
    : Minion('4', "elf_giant", 3500, 6, 0.2f, 250, 3, 35, group, battle, parent)
{
    if(group == 1) setTarget(2);
    else setTarget(1);
}
