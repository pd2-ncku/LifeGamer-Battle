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

    void addCurrentMinion(QJsonObject m);
    void addNewMinion(QJsonObject m);
    void addBuilding(QJsonObject b);

    void clear();
    void sendMap();
    void sendEnd(int winner);

signals:

public slots:

private:
    QString p1;
    QString p2;

    QJsonObject cmd;
    QJsonArray current_minion;
    QJsonArray new_minion;
    QJsonArray buildings;

    QNetworkAccessManager *renderServer;
    QString hostAddress;
    int port;
};

#endif // RENDERCOMMUNICATOR_H
