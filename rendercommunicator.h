#ifndef RENDERCOMMUNICATOR_H
#define RENDERCOMMUNICATOR_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QString>

class RenderCommunicator : public QObject
{
    Q_OBJECT
public:
    explicit RenderCommunicator(QObject *parent = 0);

    void setP1(QString name);
    void setP2(QString name);

    void setP1Hand(int *hand);
    void setP2Hand(int *hand);

    void addCurrentMinion(QJsonObject m);
    void addNewMinion(QJsonObject m);
    void addBuilding(QJsonObject b);

    void clear();
    void sendMap(int time, int p1_mana, int p2_mana);
    void sendEnd(int winner, int p1_destroy, int p2_destroy);

signals:

public slots:

private:
    QString p1;
    QString p2;

    int p1_hand[4];
    int p2_hand[4];

    QJsonObject cmd;
    QJsonArray current_minion;
    QJsonArray new_minion;
    QJsonArray buildings;

    QNetworkAccessManager *renderServer;
    QString hostAddress;
    int port;

    const QString minion_name[24];
};

#endif // RENDERCOMMUNICATOR_H
