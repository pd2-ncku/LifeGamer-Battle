#include "elfgiant.h"

ElfGiant::ElfGiant(int group, Battle *battle, QObject *parent)
    : Minion('4', "elf_giant", 1500, 7, 0.2f, 200, 2, 45, group, battle, parent)
{
    if(group == 1) setTarget(2);
    else setTarget(1);
}
