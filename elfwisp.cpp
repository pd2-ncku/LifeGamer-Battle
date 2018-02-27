#include "elfwisp.h"

#include <QtMath>

ElfWisp::ElfWisp(int group, Battle *battle, QObject *parent)
    : Minion('5', "elf_wisp", 250, 1, 0.3f, -5, 4, 3, group, battle, parent)
{
    setTarget(group);
}
