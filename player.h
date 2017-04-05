#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QTextStream>

#include <iostream>
using namespace std;

class Player : public QProcess
{
    Q_OBJECT
public:
    Player(QObject *parent = 0);
    int mana;
    int deck[4];
    int todraw[4];
    bool ready;
    QString cmd;

    bool reg();
    void addMana(int amount);

signals:
    void endGame();

private slots:
    void readCommand();

private:
    char enabled[25];
};

#endif // PLAYER_H
