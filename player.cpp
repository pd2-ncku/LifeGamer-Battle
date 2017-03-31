#include "player.h"

Player::Player(QObject *parent):QProcess(parent), mana(5), ready(false)
{
    cmd.clear();
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(readCommand()));
}

void Player::reg()
{
    int buf[8];
    QTextStream sbstream(&cmd);

    /*if(echoCommand) {
        cout << cmd.toStdString();
    }*/
    if(cmd.length() != 16) {
        cerr << "Card choose fail" << endl;
        cout << "\033[1;32;31mDeck registration error: command format error.\033[m" << endl;
        emit endGame();
        return;
    }
    for(int i = 0;i < 8;++i) {
        sbstream >> buf[i];
        if(buf[i] <= 0 || buf[i] > 9) {
            cerr << "Card choose fail" << endl;
            cout << "\033[1;32;31mDeck registration error: no such minion.\033[m" << endl;
            emit endGame();
            return;
        }
        for(int j = 0;j < i;j++) {
            if(buf[i] == buf[j]) {
                cerr << "Card choose fail" << endl;
                cout << "\033[1;32;31mDeck registration error: duplicated minion.\033[m" << endl;
                emit endGame();
                return;
            }
        }
    }

    for(int i = 0;i < 4;i++) {
        deck[i] = buf[i];
        todraw[i] = buf[i + 4];
    }
    cerr << "Card choose success" << endl;
    cout << "\033[1;32;32mDeck registration success.\033[m" << endl;
    cmd.clear();
    ready = true;
}

void Player::addMana(int amount)
{
    mana += amount;
    if(mana > 10) mana = 10;
}

void Player::readCommand()
{
    cmd += readAllStandardOutput();
}
