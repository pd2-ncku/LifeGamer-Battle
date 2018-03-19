#include "elfwisp.h"

#include <QtMath>

ElfWisp::ElfWisp(int group, Battle *battle, QObject *parent)
    : Minion('5', "elf_wisp", 160, 1, 0.3f, -15, 3, 5, group, battle, parent)
{
    setTarget(group);
}
