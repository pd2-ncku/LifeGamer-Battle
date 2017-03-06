#include "battle.h"

#include <QDebug>
#include <cstdio>

#include "unit.h"
#include "minion.h"
#include "tower.h"

Battle::Battle(QObject *parent) : QObject(parent),
    countdown(3*60*10),
    synchrogazer(new QTimer),
    started(false),
    mana_comp1(5),
    mana_comp2(5)
{
    synchrogazer->setTimerType(Qt::PreciseTimer);
    initMap();

    connect(synchrogazer, SIGNAL(timeout()), this, SLOT(clk()));
    synchrogazer->start(100);
}

Battle::~Battle()
{
    delete synchrogazer;
}

void Battle::startBattle()
{
    setCompetitor(QString("/home/hmkrl/run"));
    started = true;
}

void Battle::setCompetitor(QString path)
{
    comp1 = new QProcess(this);
    comp1->start(path);
    connect(comp1, SIGNAL(readyReadStandardOutput()), this, SLOT(readChildProcess()));
}

void Battle::readChildProcess()
{
    qDebug() << comp1->readAllStandardOutput();
}

void Battle::initMap()
{
    /* remove all units */
    UnitList.clear();

    /* Map boundary initialization */
    for(int i = 0;i < 22;++i) {
        for(int j = 0;j < 52;++j) {
            if(!i || !j || i == 21 || j == 51) {
                map[i][j] = '#';
            }
            else {
                map[i][j] = ' ';
            }
        }
    }

    /* River and bridge */
    for(int i = 24;i < 28;++i) {
        for(int j = 1;j < 4;++j) {
            map[j][i] = '#';
        }
        for(int j = 8;j < 14;++j) {
            map[j][i] = '#';
        }
        for(int j = 18;j < 21;++j) {
            map[j][i] = '#';
        }
    }

    /*  Towers */
    for(int i = 1;i <= 6;++i){
        initTower(i);
    }

}

void Battle::initTower(int SN)
{
    int x, y, size, group, p_x, p_y; /* p_x p_y is for attack detection */
    switch(SN) { /* 123:group1, 456:group2, 2 and 5 are large tower */
    case 1:
        x = 3;y = 7;size = 4;group = 1;p_x = 6;p_y = 10;
        break;
    case 2:
        x = 8;y = 3;size = 6;group = 1;p_x = 10;p_y = 8;
        break;
    case 3:
        x = 15;y = 7;size = 4;group = 1;p_x = 15;p_y = 10;
        break;
    case 4:
        x = 3;y = 41;size = 4;group = 2;p_x = 6;p_y = 41;
        break;
    case 5:
        x = 8;y = 43;size = 6;group = 2;p_x = 11;p_y = 43;
        break;
    case 6:
        x = 15;y = 41;size = 4;group = 2;p_x = 15;p_y = 41;
        break;
    }

    for(int i = 0;i < size;++i) {
        for(int j = 0;j < size;++j) {
            map[x+i][y+j] = '#';
        }
    }

    Tower *newTower;
    newTower = new Tower(1000, 100, 1, 10, 0, group, this, this);
    newTower->setPoint(p_x, p_y);
    newTower->SN = SN;
    UnitList.append(static_cast<Unit*>(newTower));
}

void Battle::destroyTower(int SN)
{
    int x, y, size;
    switch(SN) { /* 123:group1, 456:group2, 2 and 5 are large tower */
    case 1:
        x = 3;y = 7;size = 4;
        break;
    case 2:
        x = 8;y = 3;size = 6;
        break;
    case 3:
        x = 15;y = 7;size = 4;
        break;
    case 4:
        x = 3;y = 41;size = 4;
        break;
    case 5:
        x = 8;y = 43;size = 6;
        break;
    case 6:
        x = 15;y = 41;size = 4;
        break;
    }

    for(int i = 0;i < size;++i) {
        for(int j = 0;j < size;++j) {
            map[x+i][y+j] = ' ';
        }
    }
}

int Battle::addMinion()
{
    Minion *newMinion = new Minion(10000, 4, 0.7f, 300, 1, 3, 0, 2, this, this);
    newMinion->setPoint(20, 40);
    UnitList.append(static_cast<Unit*>(newMinion));

    //newMinion = new Minion(1000, 4, 4.0f, 300, 1, 5, 0, 1, this, this);
    //newMinion->setPoint(15, 5);
    //UnitList.append(static_cast<Unit*>(newMinion));

}

void Battle::clk()
{
    if(started) { /* both ready, battle started */
        countdown -= 1;

        /* Mana regeneration */
        if(!(countdown % 10)) {
            int regen = countdown < 600 ? 2 : 1;
            if(mana_comp1 < 10) mana_comp1 += regen;
            if(mana_comp2 < 10) mana_comp2 += regen;
        }

        //============ FOR DEBUG ==============
        if(countdown == 1770) {
            addMinion();
        }

        for(auto it = UnitList.begin();it != UnitList.end();++it) {
            if((*it)->getCurrentHp() <= 0) { /* remove dead units first */
                if(Tower *t = dynamic_cast<Tower*>(*it)) {
                    destroyTower(t->SN);
                }
                else {
                    Minion *m = dynamic_cast<Minion*>(*it);
                    map[m->fixed_x][m->fixed_y] = ' ';
                }

                it = UnitList.erase(it);
            }
            else {
                (*it)->active();
            }
        }

        for(int i = 0;i < 22;++i) {
            for(int j = 0;j < 52;++j) {
                putchar(map[i][j]);
            }
            putchar('\n');
        }
        //qDebug() << "write";
        //qDebug() << comp1->write("test\n");
    }
}
