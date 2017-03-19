#include "elfwisp.h"

ElfWisp::ElfWisp(int group, Battle *battle, QObject *parent)
    : Minion('5', "elf_wisp", 300, 1, 0.3f, -1, 4, group, battle, parent)
{
    setTarget(group);
}
