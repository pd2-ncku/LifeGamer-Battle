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

Battle::Battle(QObject *parent) : QObject(parent),
    countdown(3*60*10),
    synchrogazer(new QTimer),
    started(false),
    p1_mana(5),
    p2_mana(5),
    serverConnection(new QNetworkAccessManager),
    displayMap(false),
    echoCommand(false),
    minion_cost{5, 3, 4, 7, 1, 4, 9, 5}
{
    synchrogazer->setTimerType(Qt::PreciseTimer);
    initMap();

    p1_cmd.clear();
    p2_cmd.clear();

    connect(synchrogazer, SIGNAL(timeout()), this, SLOT(clk()));
    connect(this, SIGNAL(decideWinLose(int)), this, SLOT(gameFinished(int)));
    connect(this, SIGNAL(playerReady(int)), this, SLOT(changePlayerState(int)));
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
    p1 = new QProcess(this);
    p1->start(path);
    connect(p1, SIGNAL(readyReadStandardOutput()), this, SLOT(readP1()));
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

    p2 = new QProcess(this);
    p2->start(path);
    connect(p2, SIGNAL(readyReadStandardOutput()), this, SLOT(readP2()));
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

void Battle::readP1()
{
    p1_cmd += p1->readAllStandardOutput();
}

void Battle::readP2()
{
    p2_cmd += p2->readAllStandardOutput();
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
    int mana, group, enemy, y_raw = y;
    if(player == 1) {
        mana = p1_mana;
        group = 1;
        enemy = 2;
        for(int i = 0;i < 4;i++) {
            if(p1_deck[i] == num) {
                inDeck = true;
                break;
            }
        }
    }
    if(player == 2) {
        mana = p2_mana;
        group = 2;
        enemy = 1;
        y = 51 - y;
        for(int i = 0;i < 4;i++) {
            if(p2_deck[i] == num) {
                inDeck = true;
                break;
            }
        }
    }
    if(!inDeck) {
        cout << "\033[1;32;31msummon minion " << num << " at " << x << " " << y << " failed: minion not in deck.\033[m" << endl;
        return SummonFailedNotInDeck;
    }
    else if(num < 1 || num > 8) {
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
        switch(num) {
        case 1:
            newMinion = new Minion('1', 1500, 5, 0.3f, 5, 3, group, enemy, this, this);
            break;
        case 2:
            newMinion = new Minion('2', 700, 3, 0.2f, -5, 5, group, group, this, this);
            break;
        case 3:
            newMinion = new Minion('3', 500, 4, 0.4f, 15, 3, group, enemy, this, this);
            break;
        case 4:
            newMinion = new Minion('4', 3500, 7, 0.2f, 1, 2, group, enemy, this, this);
            break;
        case 5:
            newMinion = new Minion('5', 300, 1, 0.3f, -1, 4, group, group, this, this);
            break;
        case 6:
            newMinion = new Minion('6', 300, 4, 0.3f, 10, 5, group, enemy, this, this);
            break;
        case 7:
            newMinion = new Minion('7', 2500, 9, 0.1f, 10, 2, group, enemy, this, this);
            break;
        case 8:
            newMinion = new Minion('8', 1500, 5, 0.1f, 20, 2, group, enemy, this, this);
            break;
        }
        newMinion->setPoint(x, y);
        UnitList.append(static_cast<Unit*>(newMinion));
        if(player == 1) {
            for(int i = 0;i < 4;i++) {
                if(p1_deck[i] == num) {
                    int rand = qrand() % 4;
                    p1_deck[i] = p1_todraw[rand];
                    p1_todraw[rand] = num;
                    p1_mana -= minion_cost[num - 1];
                }
            }
        }
        if(player == 2) {
            for(int i = 0;i < 4;i++) {
                if(p2_deck[i] == num) {
                    int rand = qrand() % 4;
                    p2_deck[i] = p2_todraw[rand];
                    p2_todraw[rand] = num;
                    p2_mana -= minion_cost[num - 1];
                }
            }
        }
        cout << "\033[1;32;32msummon minion " << num << " at " << x << " " << y << " success.\033[m" << endl;
        return SummonSuccess;
    }
}

void Battle::p1_reg()
{
    int buf[8];
    QTextStream sbstream(&p1_cmd);

    if(echoCommand) {
        cout << p1_cmd.toStdString();
    }
    if(p1_cmd.length() != 16) {
        cout << "\033[1;32;31mDeck registration error: command format error.\033[m" << endl;
        p1->terminate();
        emit endGame();
        return;
    }
    for(int i = 0;i < 8;++i) {
        sbstream >> buf[i];
        if(buf[i] <= 0 || buf[i] > 8) {
            cout << "\033[1;32;31mDeck registration error: no such minion.\033[m" << endl;
            p1->terminate();
            emit endGame();
            return;
        }
        for(int j = 0;j < i;j++) {
            if(buf[i] == buf[j]) {
                cout << "\033[1;32;31mDeck registration error: duplicated minion.\033[m" << endl;
                p1->terminate();
                emit endGame();
                return;
            }
        }
    }

    for(int i = 0;i < 4;i++) {
        p1_deck[i] = buf[i];
        p1_todraw[i] = buf[i + 4];
    }
    cout << "\033[1;32;32mDeck registration success.\033[m" << endl;
    p1_cmd.clear();
    emit playerReady(1);
}

void Battle::p2_reg()
{
    int buf[8];
    QTextStream sbstream(&p2_cmd);

    if(echoCommand) {
        cout << p2_cmd.toStdString();
    }
    if(p2_cmd.length() != 16) {
        cout << "\033[1;32;31mDeck registration error: command format error.\033[m" << endl;
        p2->terminate();
        emit endGame();
        return;
    }
    for(int i = 0;i < 8;++i) {
        sbstream >> buf[i];
        if(buf[i] <= 0 || buf[i] > 8) {
            cout << "\033[1;32;31mDeck registration error: no such minion.\033[m" << endl;
            p2->terminate();
            emit endGame();
            return;
        }
        for(int j = 0;j < i;j++) {
            if(buf[i] == buf[j]) {
                cout << "\033[1;32;31mDeck registration error: duplicated minion.\033[m" << endl;
                p2->terminate();
                emit endGame();
                return;
            }
        }
    }

    for(int i = 0;i < 4;i++) {
        p2_deck[i] = buf[i];
        p2_todraw[i] = buf[i + 4];
    }
    cout << "\033[1;32;32mDeck registration success.\033[m" << endl;
    p2_cmd.clear();
    emit playerReady(2);
}

void Battle::clk()
{
    if(countdown == 0)
        emit decideWinLose(0);
    if(!started) {
        if(p1_cmd.length()) {
            p1_reg();
        }
        if(p2_cmd.length()) {
            p2_reg();
        }
        return;
    }
    QJsonObject cmd, minion, tower;
    QJsonArray current_minion, new_minion, buildings;

    for(auto it = UnitList.begin();it != UnitList.end();++it) {
        if(Minion *m = dynamic_cast<Minion*>(*it)) {
            minion["belong"] = "p" + QString::number(m->group);
            minion["name"] = "orge1";
            minion["type"] = "orge";
            minion["status"] = QString::number(m->getHpChange());
            minion["move"] = QString::number(m->stat);
            minion["loc_x"] = QString::number(m->fixed_y);
            minion["loc_y"] = QString::number(m->fixed_x);
            current_minion.append(minion);
            //debug=============================
            //new_minion.append(minion);
        }
        else { /* Tower */
            Tower* t = dynamic_cast<Tower*>(*it);
            QString name("p");
            name += QString::number(t->group);
            name += "_";
            switch(t->SN) {
            case 1:case 4:
                name += "TOP";
                break;
            case 2:case 5:
                name += "MIDDLE";
                break;
            case 3: case 6:
                name += "BOTTOM";
                break;
            }

            tower["name"] = name;
            tower["status"] = QString::number(t->getHpChange());
            buildings.append(tower);
        }
    }

    cmd["p1"] = "kevin";
    cmd["p2"] = "eric";
    cmd["cmd"] = "battle";
    cmd["current_minion"] = current_minion;
    cmd["new_minion"] = new_minion;
    cmd["buildings"] = buildings;

    QString msg(QJsonDocument(cmd).toJson(QJsonDocument::Compact));

    QUrl host("http://localhost:3001/game_cmd");
    QUrlQuery qry;
    qry.addQueryItem("p1", "kevin");
    qry.addQueryItem("p2", "eric");
    host.setQuery(qry);
    QNetworkRequest req(host);

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setHeader(QNetworkRequest::ContentLengthHeader, msg.length());

    serverConnection->post(req, QByteArray(msg.toStdString().c_str()));

    /* map info send to render server finished */
    emit signalLogHp();

    countdown -= 1;

    /* Mana regeneration */
    if(!(countdown % 10)) {
        int regen = countdown < 600 ? 2 : 1;
        if(p1_mana < 10) p1_mana += regen;
        if(p2_mana < 10) p2_mana += regen;
    }

    /* summon new minion */
    if(p1_cmd.length()) {
        if(echoCommand) {
            cout << "\033[1;36m" << p1_cmd.toStdString() << "\033[m";
        }
        int command, minion, x, y;
        QTextStream compcmd(&p1_cmd);
        while(!compcmd.atEnd()) {
            compcmd >> command;
            if(command == 0) break;
            else {
                compcmd >> minion >> x >> y;
                addMinion(1, minion, x, y);
            }
        }
        p1_cmd.clear();
    }
    if(p2_cmd.length()) {
        if(echoCommand) {
            cout << "\033[1;36m" << p2_cmd.toStdString() << "\033[m";
        }
        int command, minion, x, y;
        QTextStream compcmd(&p2_cmd);
        while(!compcmd.atEnd()) {
            compcmd >> command;
            if(command == 0) break;
            else {
                compcmd >> minion >> x >> y;
                addMinion(2, minion, x, y);
            }
        }
        p2_cmd.clear();
    }
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

    p1_toSendst << countdown / 10 << ' ' << p1_mana;
    p2_toSendst << countdown / 10 << ' ' << p2_mana;

    for(int i = 0;i < 4;i++) {
        p1_toSendst << ' ' << p1_deck[i];
        p2_toSendst << ' ' << p2_deck[i];
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

void Battle::changePlayerState(int player)
{
    static bool p1_ready = false;
    static bool p2_ready = false;

    if(player == 1) p1_ready = true;
    if(player == 2) p2_ready = true;

    if(p1_ready && p2_ready) {
        startBattle();
    }
}

void Battle::gameFinished(int SN)
{
    synchrogazer->stop();
    if(SN == 2) cout << "You Lose" << endl;
    else if(SN == 5) cout << "You Win" << endl;
    else {
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
        if(!tower[1]) {
            cout << "You Lose" << endl;
        }
        else if(!tower[4]) {
            cout << "You Win" << endl;
        }
        else {
            if(cnt1 > cnt2) {
                cout << "You Win" << endl;
            }
            else if(cnt1 == cnt2) {
                cout << "Draw" << endl;
            }
            else {
                cout << "You Lose" << endl;
            }
        }
    }
    emit endGame();
}
