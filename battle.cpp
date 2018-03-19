#include "battle.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QByteArray>
#include <QTextStream>
#include <QCoreApplication>
#include <QFile>

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
    judge(new JudgeCommunicator(this)),
    displayMap(false),
    echoCommand(false),
    testRegister(false),
    testInteract(false),
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
    connect(this, &Battle::endGame, this, &Battle::postSolve);

    /* player signals */
    connect(p1, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(p1_error(QProcess::ProcessError)));

    connect(p2, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(p2_error(QProcess::ProcessError)));

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

    cout << "Starting Player 1..." << endl;
}

bool Battle::setP2(QString path)
{
    p2->start(path);

    cout << "Starting Player 2..." << endl;
}

void Battle::setMapOutput()
{
    displayMap = true;
}

void Battle::setEchoOutput()
{
    echoCommand = true;
}

void Battle::setTestRegister()
{
    testRegister = true;
}

void Battle::setTestInteract()
{
    testInteract = true;
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
        QFile file;
        file.setFileName("crashed");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
        emit endGame(1);
    }
    else if(error == QProcess::FailedToStart) {
        cout << "\033[1;32;31mError: cannot start Player 1!\033[m" << endl;
        emit endGame(1);
    }
}

void Battle::p2_error(QProcess::ProcessError error)
{
    if(!judged && error == QProcess::Crashed) {
        judged = true;
        cerr << "Player 2 fault" << endl;
        cerr << "Player 1 win" << endl;
        emit endGame(0);
    }
    else if(error == QProcess::FailedToStart) {
        cout << "\033[1;32;31mError: cannot start Player 2!\033[m" << endl;
        emit endGame(0);
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
    int x, y, size, group, target, p_x, p_y, atk, attackRange, attackDelay, hp; /* p_x p_y is for attack detection */
    switch(SN) { /* 123:group1, 456:group2, 2 and 5 are large tower */
    case 1:
        x = 3;y = 7;size = 4;group = 1;p_x = 5;p_y = 10;hp = 2000;atk = 100;attackRange=6;attackDelay = 9;
        break;
    case 2:
        x = 8;y = 3;size = 6;group = 1;p_x = 10;p_y = 8;hp = 2500;atk = 200;attackRange=7;attackDelay = 12;
        break;
    case 3:
        x = 15;y = 7;size = 4;group = 1;p_x = 16;p_y = 10;hp = 2000;atk = 100;attackRange=6;attackDelay = 9;
        break;
    case 4:
        x = 3;y = 41;size = 4;group = 2;p_x = 5;p_y = 41;hp = 2000;atk = 100;attackRange=6;attackDelay = 9;
        break;
    case 5:
        x = 8;y = 43;size = 6;group = 2;p_x = 11;p_y = 43;hp = 2500;atk = 200;attackRange=7;attackDelay = 12;
        break;
    case 6:
        x = 15;y = 41;size = 4;group = 2;p_x = 16;p_y = 41;hp = 2000;atk = 100;attackRange=6;attackDelay = 9;
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
    newTower = new Tower(hp, atk, attackRange, attackDelay, group, target, this, this);
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
        if(player == 1) cout << "\033[1;32;31msummon minion " << static_cast<char>(num + '0') << " at " << x << " " << y << " failed: minion not in deck.\033[m" << endl;
        return SummonFailedNotInDeck;
    }
    else if(num < 1 || !minion_cost[num - 1]) {
        if(player == 1) cout << "\033[1;32;31msummon minion " << static_cast<char>(num + '0') << " at " << x << " " << y << " failed: no such minion.\033[m" << endl;
        return SummonFailedUnknowMinion;
    }
    else if(mana < minion_cost[num - 1]) {
        if(player == 1) cout << "\033[1;32;31msummon minion " << static_cast<char>(num + '0') << " at " << x << " " << y << " failed: no enough mana.\033[m" << endl;
        return SummonFailedOM;
    }
    else if(y_raw > 24 || y_raw < 1 || x > 20 || x < 1) {
        if(player == 1) cout << "\033[1;32;31msummon minion " << static_cast<char>(num + '0') << " at " << x << " " << y << " failed: out of range.\033[m" << endl;
        return SummonFailedOutOfRange;
    }
    else if(map[x][y] != ' ') {
        if(player == 1) cout << "\033[1;32;31msummon minion " << static_cast<char>(num + '0') << " at " << x << " " << y << " failed: map occupied.\033[m" << endl;
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
        if(player == 1) cout << "\033[1;32;32msummon minion " << static_cast<char>(num + '0') << " at " << x << " " << y << " success.\033[m" << endl;
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
            if(p1->reg()) {
                render->setP1Hand(p1->deck);
                if(testRegister) {
                    emit endGame(0);
                }
            }
            else {
                judged = true;
                cerr << "Player 1 fault" << endl;
                cerr << "Player 2 win" << endl;
                if(testRegister) {
                    emit endGame(1);
                }
            }
        }
        if(p2->cmd.length()) {
            if(p2->reg()) {
                render->setP2Hand(p2->deck);
            }
            else {
                judged = true;
                cerr << "Player 2 fault" << endl;
                cerr << "Player 1 win" << endl;
            }
        }
        if(p1->ready && p2->ready) {
            startBattle();
        }
        if(cnt++ > 10) { /* register timeout is 1 second */
            cerr << "timeout" << endl;
            if(!p1->ready) {
                cerr << "Card choose fail" << endl;
                judged = true;
                cerr << "Player 1 fault" << endl;
                cerr << "Player 2 win" << endl;
                emit endGame(1);
            }
            if(!p2->ready) {
                //cerr << "Card choose fail" << endl;
                judged = true;
                cerr << "Player 2 fault" << endl;
                cerr << "Player 1 win" << endl;
                emit endGame(0);
            }
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

    if(displayMap || echoCommand) cout << "\033c";

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
                    p1_judge = true;
                    if(testInteract) {
                        emit endGame(0);
                    }
                }
                compcmd >> trash >> tmp >> x >> y;
                minion_ID = tmp - '0';
                addMinion(1, minion_ID, x, y);
            }
            else {
                if(testInteract) {
                    emit endGame(1);
                }
                break;
            }
        }
        p1->cmd.clear();
    }
    if(p2->cmd.length()) {
        char tmp, trash;
        int command, minion_ID, x, y;
        QTextStream compcmd(&p2->cmd);
        while(!compcmd.atEnd()) {
            compcmd >> command;
            if(command == 0) break;
            else if(command == 1) {
                compcmd >> trash >> tmp >> x >> y;
                minion_ID = tmp - '0';
                addMinion(2, minion_ID, x, y);
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

    /* The string will send to player */
    QString p1_toSend, p2_toSend;
    QTextStream p1_toSendst(&p1_toSend);
    QTextStream p2_toSendst(&p2_toSend);

    p1_toSendst << "BEGIN\n";
    p1_toSendst << "TIME " << countdown / 10 << '\n';
    p1_toSendst << "MANA " << p1->mana << '\n';

    p2_toSendst << "BEGIN\n";
    p2_toSendst << "TIME " << countdown / 10 << '\n';
    p2_toSendst << "MANA " << p2->mana << '\n';

    p1_toSendst << "DECK";
    p2_toSendst << "DECK";
    for(int i = 0;i < 4;i++) {
        p1_toSendst << ' ' << static_cast<char>(p1->deck[i] + '0');
        p2_toSendst << ' ' << static_cast<char>(p2->deck[i] + '0');
    }
    p1_toSendst << '\n';
    p2_toSendst << '\n';

    /* update map_hp information */
    for(auto it = UnitList.begin();it != UnitList.end();++it) {
        if(Tower *t = dynamic_cast<Tower*>(*it)) {
            modifyTowerHp(t->SN, t->getHpRatio());

            /* For player */
            if(t->SN <= 3) { /* Left towers */
                p1_toSendst << "TOWER " << t->SN << ' ' << t->getHpRatio() << "\n";
                p2_toSendst << "TOWER " << t->SN + 3 << ' ' << t->getHpRatio() << "\n";
            }
            else {
                p1_toSendst << "TOWER " << t->SN << ' ' << t->getHpRatio() << "\n";
                p2_toSendst << "TOWER " << t->SN - 3 << ' ' << t->getHpRatio() << "\n";
            }
        }
        else {
            Minion *m = dynamic_cast<Minion*>(*it);
            int index = m->getHpRatio()/10;
            if(index < 0) index = 0;
            map_hp[m->fixed_x][m->fixed_y] = "0123456789A"[index];

            /* For player */
            if(m->group == 1) {
                p1_toSendst << "FRIEND ";
                p2_toSendst << "ENEMY ";
            }
            else {
                p1_toSendst << "ENEMY ";
                p2_toSendst << "FRIEND ";
            }

            int tmp_hp = m->getHpRatio();
            if(tmp_hp < 0) tmp_hp = 0;

            p1_toSendst << m->minion_num
                        << " AT "
                        << m->fixed_x << ',' << m->fixed_y
                        << " HP "
                        << tmp_hp
                        << "\n";

            p2_toSendst << m->minion_num
                        << " AT "
                        << m->fixed_x << ',' << 51 - m->fixed_y
                        << " HP "
                        << tmp_hp
                        << "\n";
        }
    }

    p1_toSendst << "END\n";
    p2_toSendst << "END\n";

    p1->write(p1_toSend.toStdString().c_str());
    p2->write(p2_toSend.toStdString().c_str());

    // cout << p1_toSend.toStdString();

    /* Build colored map */
    QString coloredMap, htmlMap;
    coloredMap.clear();
    coloredMap.clear();
    QTextStream ts(&coloredMap);
    QTextStream ts_html(&htmlMap);

    ts_html << "Time: " << countdown / 10 << "    "
            << "Mana: " << p1->mana << "    "
            << "Deck:";
    for(int i = 0;i < 4;i++) {
        ts_html << ' ' << static_cast<char>(p1->deck[i] + '0');
    }
    ts_html << "<br>";

    for(int i = 0;i < 22;i++) {
        for(int j = 0;j < 52;j++) {
            if(map_hp[i][j] == ' ' || map_hp[i][j] == '#') {
                ts << map[i][j];
                ts_html << map[i][j];
            }
            else {
                switch(map_hp[i][j]) {
                case 'A': case '9': case '8': case '7': case '6':
                    ts << GREEN;
                    ts_html << "<span style='color:#8AE234;'>";
                    break;
                case '5': case '4': case '3':
                    ts << YELLOW;
                    ts_html << "<span style='color:#FCE94F;'>";
                    break;
                case '2': case '1': case '0':
                    ts << RED;
                    ts_html << "<span style='color:#ED2929;'>";
                    break;
                }

                if(map[i][j] == '#') {
                    ts << map_hp[i][j] << NONE;
                    ts_html << map_hp[i][j] << "</span>";
                }
                else {
                    ts << map[i][j] << NONE;
                    ts_html << map[i][j] << "</span>";
                }
            }
        }
        ts << '\n';
        ts_html << "<br>";
    }

    if(displayMap) {
        cout << "Time: " << countdown / 10 << "    "
             << "Mana: " << p1->mana << "    "
             << "Deck:";
        for(int i = 0;i < 4;i++) {
            cout << ' ' << static_cast<char>(p1->deck[i] + '0');
        }
        cout << endl;
        cout << coloredMap.toStdString();
    }
    if(!(countdown % 10)) {
        judge->sendMap(htmlMap);
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
    if(winner == 2) {
        judge->sendResult("You Lost...");
        emit endGame(1);
    }
    else {
        judge->sendResult("You Win!!!");
        emit endGame(0);
    }
}

void Battle::postSolve(int retval)
{
    judged = true;
    synchrogazer->stop();

    p1->kill();
    p2->kill();
    p1->waitForFinished(1000);
    p2->waitForFinished(1000);

    judge->waitResultFinished(3000);

    QCoreApplication::exit(retval);
}
