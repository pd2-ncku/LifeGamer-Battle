#include "battle.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QByteArray>
#include <QTextStream>

#include <iostream>
using namespace std;

#include "unit.h"
#include "minion.h"
#include "tower.h"

/* minions */
#include "humanknight.h"
#include "humanpriest.h"
#include "humanthief.h"
#include "humanrifleman.h" // Add Rifleman
#include "elfgiant.h"
#include "elfwisp.h"
#include "elfarcher.h"
#include "elfdancer.h"
#include "undeadsamurai.h"
#include "sgram.h"

Battle::Battle(QObject *parent) : QObject(parent),
    countdown(3*60*10),
    synchrogazer(new QTimer),
    started(false),
    render(new RenderCommunicator(this)),
    displayMap(false),
    echoCommand(false),
    judged(false),
    p1(new Player(this)),
    p2(new Player(this)),
    minion_cost{5, 3, 4, 7, 1, 4, 9, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0}
             /* 1, 2, 3, 4, 5, 6, 7, 8, 9, :, ;, <, =, >, ?, @, A, B, C, D, E, F, G, H */
{
    synchrogazer->setTimerType(Qt::PreciseTimer);
    initMap();

    /* ticking signal */
    connect(synchrogazer, SIGNAL(timeout()), this, SLOT(clk()));

    /* internal signal communication */
    connect(this, SIGNAL(decideWinLose(int)), this, SLOT(gameFinished(int)));
    connect(this, SIGNAL(endGame()), this, SLOT(postSolve()));

    /* player signals */
    connect(p1, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(p1_error(QProcess::ProcessError)));
    connect(p1, SIGNAL(endGame()), this, SLOT(postSolve()));

    connect(p2, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(p2_error(QProcess::ProcessError)));
    connect(p2, SIGNAL(endGame()), this, SLOT(postSolve()));

    synchrogazer->start(100);
}

Battle::~Battle()
{
    delete synchrogazer;
}

void Battle::startBattle()
{
    started = true;
}

bool Battle::setP1(QString path)
{
    p1->start(path);

    cout << "Starting your program..." << endl;
    if(!p1->waitForStarted(1000)) {
        cout << "\033[1;32;31mError: cannot start your program!\033[m" << endl;
        return false;
    }
    else {
        cout << "\033[1;32;32mYour program has started.\033[m" << endl;
        return true;
    }
}

bool Battle::setP2(QString path)
{
    p2->start(path);

    if(!p2->waitForStarted(1000)) {
        cout << "\033[1;32;31mError: cannot start your program!\033[m" << endl;
        return false;
    }
    else {
        return true;
    }
}

void Battle::setMapOutput()
{
    displayMap = true;
}

void Battle::setEchoOutput()
{
    echoCommand = true;
}

void Battle::setP1Name(QString name)
{
    render->setP1(name);
}

void Battle::setP2Name(QString name)
{
    render->setP2(name);
}

void Battle::p1_error(QProcess::ProcessError error)
{
    if(!judged && error == QProcess::Crashed) {
        judged = true;
        cerr << "Player 1 fault" << endl;
        cerr << "Player 2 win" << endl;
        emit endGame();
    }
}

void Battle::p2_error(QProcess::ProcessError error)
{
    if(!judged && error == QProcess::Crashed) {
        judged = true;
        cerr << "Player 2 fault" << endl;
        cerr << "Player 1 win" << endl;
        emit endGame();
    }
}

void Battle::initMap()
{
    /* remove all units */
    UnitList.clear();

    /* Map boundary initialization */
    for(int i = 0;i < 22;++i) {
        for(int j = 0;j < 53;++j) {
            if(!i || !j || i == 21 || j == 51) {
                map[i][j] = '#';
                map_hp[i][j] = '#';
            }
            else {
                map[i][j] = ' ';
                map_hp[i][j] = ' ';
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

    /* NULL end string */
    for(int i = 0;i < 22;++i) {
        map[i][52] = '\0';
        map_hp[i][52] = '\0';
    }

}

void Battle::initTower(int SN)
{
    int x, y, size, group, target, p_x, p_y, atk, hp; /* p_x p_y is for attack detection */
    switch(SN) { /* 123:group1, 456:group2, 2 and 5 are large tower */
    case 1:
        x = 3;y = 7;size = 4;group = 1;p_x = 5;p_y = 10;hp = 4000;atk = 4;
        break;
    case 2:
        x = 8;y = 3;size = 6;group = 1;p_x = 10;p_y = 8;hp = 5000;atk = 5;
        break;
    case 3:
        x = 15;y = 7;size = 4;group = 1;p_x = 16;p_y = 10;hp = 4000;atk = 4;
        break;
    case 4:
        x = 3;y = 41;size = 4;group = 2;p_x = 5;p_y = 41;hp = 4000;atk = 4;
        break;
    case 5:
        x = 8;y = 43;size = 6;group = 2;p_x = 11;p_y = 43;hp = 5000;atk = 5;
        break;
    case 6:
        x = 15;y = 41;size = 4;group = 2;p_x = 16;p_y = 41;hp = 4000;atk = 4;
        break;
    }
    if(group == 1) target = 2;
    else target = 1;

    for(int i = 0;i < size;++i) {
        for(int j = 0;j < size;++j) {
            map[x+i][y+j] = '#';
            map_hp[x+i][y+j] = '9';
        }
    }

    Tower *newTower;
    newTower = new Tower(hp, atk, 5, group, target, this, this);
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
            map_hp[x+i][y+j] = ' ';
        }
    }
    if(SN == 2 || SN == 5) {
        emit decideWinLose(SN);
    }
}

void Battle::modifyTowerHp(int SN, int hpRatio)
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
    int index = hpRatio/10;
    if(index < 0) index = 0;

    for(int i = 0;i < size;++i) {
        for(int j = 0;j < size;++j) {
            map_hp[x+i][y+j] = "0123456789A"[index];
        }
    }
}

int Battle::addMinion(int player, int num, int x, int y)
{
    bool inDeck = false;
    int mana, group, y_raw = y;
    if(player == 1) {
        mana = p1->mana;
        group = 1;
        for(int i = 0;i < 4;i++) {
            if(p1->deck[i] == num) {
                inDeck = true;
                break;
            }
        }
    }
    if(player == 2) {
        mana = p2->mana;
        group = 2;
        y = 51 - y;
        for(int i = 0;i < 4;i++) {
            if(p2->deck[i] == num) {
                inDeck = true;
                break;
            }
        }
    }
    if(!inDeck) {
        cout << "\033[1;32;31msummon minion " << num << " at " << x << " " << y << " failed: minion not in deck.\033[m" << endl;
        return SummonFailedNotInDeck;
    }
    else if(num < 1 || !minion_cost[num - 1]) {
        cout << "\033[1;32;31msummon minion " << num << " at " << x << " " << y << " failed: no such minion.\033[m" << endl;
        return SummonFailedUnknowMinion;
    }
    else if(mana < minion_cost[num - 1]) {
        cout << "\033[1;32;31msummon minion " << num << " at " << x << " " << y << " failed: no enough mana.\033[m" << endl;
        return SummonFailedOM;
    }
    else if(y_raw > 24 || y_raw < 1 || x > 20 || x < 1) {
        cout << "\033[1;32;31msummon minion " << num << " at " << x << " " << y << " failed: out of range.\033[m" << endl;
        return SummonFailedOutOfRange;
    }
    else if(map[x][y] != ' ') {
        cout << "\033[1;32;31msummon minion " << num << " at " << x << " " << y << " failed: map occupied.\033[m" << endl;
        return SummonFailedOccupied;
    }
    else {
        Minion *newMinion;
        switch(num + '0') {
        case '1':
            newMinion = new HumanKnight(group, this, this);
            break;
        case '2':
            newMinion = new HumanPriest(group, this, this);
            break;
        case '3':
            newMinion = new HumanThief(group, this, this);
            break;
        case '4':
            newMinion = new ElfGiant(group, this, this);
            break;
        case '5':
            newMinion = new ElfWisp(group, this, this);
            break;
        case '6':
            newMinion = new ElfArcher(group, this, this);
            break;
        case '7':
            newMinion = new UndeadSamurai(group, this, this);
            break;
        case '8':
            newMinion = new Sgram(group, this, this);
            break;
        case '9':
            newMinion = new ElfDancer(group, this, this);
            break;
        case 'C':
            newMinion = new HumanRifleman(group, this, this);
            break;
        }
        newMinion->setPoint(x, y);
        UnitList.append(static_cast<Unit*>(newMinion));
        if(player == 1) {
            for(int i = 0;i < 4;i++) {
                if(p1->deck[i] == num) {
                    int rand = qrand() % 4;
                    p1->deck[i] = p1->todraw[rand];
                    p1->todraw[rand] = num;
                    p1->mana -= minion_cost[num - 1];
                }
            }

            render->setP1Hand(p1->deck);
        }
        if(player == 2) {
            for(int i = 0;i < 4;i++) {
                if(p2->deck[i] == num) {
                    int rand = qrand() % 4;
                    p2->deck[i] = p2->todraw[rand];
                    p2->todraw[rand] = num;
                    p2->mana -= minion_cost[num - 1];
                }
            }

            render->setP2Hand(p2->deck);
        }
        cout << "\033[1;32;32msummon minion " << num << " at " << x << " " << y << " success.\033[m" << endl;
        render->addNewMinion(newMinion->toJsonObject(true));
        return SummonSuccess;
    }
}

void Battle::clk()
{
    static bool p1_judge = false;
    static int cnt = 0;
    if(countdown == 0)
        emit decideWinLose(0);
    if(!started) {
        if(p1->cmd.length()) {
            p1->reg();
            render->setP1Hand(p1->deck);
        }
        if(p2->cmd.length()) {
            p2->reg();
            render->setP2Hand(p2->deck);
        }
        if(p1->ready && p2->ready) {
            startBattle();
        }
        if(cnt++ > 10) { /* register timeout is 1 second */
            cerr << "timeout" << endl;
            if(!p1->ready) {
                cerr << "Card choose fail" << endl;
            }
            if(!p2->ready) {
                //cerr << "Card choose fail" << endl;
            }
            emit endGame();
        }
        return;
    }

    for(auto it = UnitList.begin();it != UnitList.end();++it) {
        if(Minion *m = dynamic_cast<Minion*>(*it)) {
            render->addCurrentMinion(m->toJsonObject());
        }
        else {
            Tower* t = dynamic_cast<Tower*>(*it);
            render->addBuilding(t->toJsonObject());
        }
    }

    countdown -= 1;

    /* Mana regeneration */
    if(!(countdown % 10)) {
        int regen = countdown < 600 ? 2 : 1;
        p1->addMana(regen);
        p2->addMana(regen);
    }

    /* summon new minion */
    if(p1->cmd.length()) {
        if(echoCommand) {
            cout << "\033[1;36m" << p1->cmd.toStdString() << "\033[m";
        }
        char tmp, trash;
        int command, minion_ID, x, y;
        QTextStream compcmd(&p1->cmd);
        while(!compcmd.atEnd()) {
            compcmd >> command;
            if(command == 0) break;
            else if(command == 1) {
                if(!p1_judge) {
                    cerr << "Interact success" << endl;
                    p1_judge = true;
                }
                compcmd >> trash >> tmp >> x >> y;
                minion_ID = tmp - '0';
                addMinion(1, minion_ID, x, y) == Battle::SummonSuccess;
            }
            else {
                cerr << "Interact fail" << endl;
                break;
            }
        }
        p1->cmd.clear();
    }
    if(p2->cmd.length()) {
        if(echoCommand) {
            cout << "\033[1;36m" << p2->cmd.toStdString() << "\033[m";
        }
        int command, minion_ID, x, y;
        QTextStream compcmd(&p2->cmd);
        while(!compcmd.atEnd()) {
            compcmd >> command;
            if(command == 0) break;
            else if(command == 1) {
                compcmd >> minion_ID >> x >> y;
                addMinion(2, minion_ID, x, y) == Battle::SummonSuccess;
            }
            else break;
        }
        p2->cmd.clear();
    }

    render->sendMap(countdown / 10, p1->mana, p2->mana);
    emit signalLogHp();

    for(auto it = UnitList.begin();it != UnitList.end();++it) {
        if((*it)->getCurrentHp() <= 0) { /* remove dead units first */
            if(Tower *t = dynamic_cast<Tower*>(*it)) {
                destroyTower(t->SN);
            }
            else {
                Minion *m = dynamic_cast<Minion*>(*it);
                map[m->fixed_x][m->fixed_y] = ' ';
                map_hp[m->fixed_x][m->fixed_y] = ' ';
            }

            it = UnitList.erase(it);
        }
        else {
            (*it)->active();
        }
    }
    /* update map_hp information */
    for(auto it = UnitList.begin();it != UnitList.end();++it) {
        if(Tower *t = dynamic_cast<Tower*>(*it)) {
            modifyTowerHp(t->SN, t->getHpRatio());
        }
        else {
            Minion *m = dynamic_cast<Minion*>(*it);
            int index = m->getHpRatio()/10;
            if(index < 0) index = 0;
            map_hp[m->fixed_x][m->fixed_y] = "0123456789A"[index];
        }
    }
    QString p1_toSend, p2_toSend;
    QTextStream p1_toSendst(&p1_toSend);
    QTextStream p2_toSendst(&p2_toSend);

    p1_toSendst << countdown / 10 << ' ' << p1->mana;
    p2_toSendst << countdown / 10 << ' ' << p2->mana;

    for(int i = 0;i < 4;i++) {
        p1_toSendst << ' ' << p1->deck[i];
        p2_toSendst << ' ' << p2->deck[i];
    }
    p1_toSendst << '\n';
    p2_toSendst << '\n';

    p1->write(p1_toSend.toStdString().c_str());
    p2->write(p2_toSend.toStdString().c_str());

    for(int i = 0;i < 22;++i) {
        p1->write(QByteArray(map[i]).append("\n"));
        for(int j = 51;j >= 0;--j) {
            p2->write(QByteArray(1, map[i][j]));
        }
        p2->write(QByteArray(1, '\n'));
    }
    for(int i = 0;i < 22;++i) {
        p1->write(QByteArray(map_hp[i]).append("\n"));
        for(int j = 51;j >= 0;--j) {
            p2->write(QByteArray(1, map_hp[i][j]));
        }
        p2->write(QByteArray(1, '\n'));
    }

    if(displayMap) {
        cout << p1_toSend.toStdString();
        for(int i = 0;i < 22;++i) {
            cout << map[i] << endl;
        }
        for(int i = 0;i < 22;++i) {
            cout << map_hp[i] << endl;
        }
    }
}

void Battle::gameFinished(int SN)
{
    int winner;
    judged = true;
    synchrogazer->stop();

    /* count for towers */
    bool tower[6];
    int cnt1 = 0, cnt2 = 0;
    for(int i = 0;i < 6;i++) {
        tower[i] = false;
    }
    for(auto it = UnitList.begin();it != UnitList.end();++it) {
        if(Tower *t = dynamic_cast<Tower*>(*it)) {
            tower[t->SN - 1] = true;
            if(t->SN <= 3) cnt1++;
            else cnt2++;
        }
    }

    if(SN == 2) {
        cerr << "Player 2 win" << endl;
        cout << "You Lose" << endl;
        winner = 2;
    }
    else if(SN == 5) {
        cerr << "Player 1 win" << endl;
        cout << "You Win" << endl;
        winner = 1;
    }
    else {
        if(!tower[1]) {
            cerr << "Player 2 win" << endl;
            cout << "You Lose" << endl;
            winner = 2;
        }
        else if(!tower[4]) {
            cerr << "Player 1 win" << endl;
            cout << "You Win" << endl;
            winner = 1;
        }
        else {
            if(cnt1 > cnt2) {
                cerr << "Player 1 win" << endl;
                cout << "You Win" << endl;
                winner = 1;
            }
            else if(cnt1 == cnt2) {
                cerr << "Tie" << endl;
                cout << "Draw" << endl;
                winner = 3;
            }
            else {
                cerr << "Player 2 win" << endl;
                cout << "You Lose" << endl;
                winner = 2;
            }
        }
    }

    render->sendEnd(winner, 3 - cnt2, 3 - cnt1);
    emit endGame();
}

void Battle::postSolve()
{
    p1->kill();
    p2->kill();
    p1->waitForFinished(1000);
    p2->waitForFinished(1000);

    emit quit();
}
