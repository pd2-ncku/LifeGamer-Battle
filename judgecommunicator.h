#ifndef JUDGECOMMUNICATOR_H
#define JUDGECOMMUNICATOR_H

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

class JudgeCommunicator : public QObject
{
    Q_OBJECT
public:
    explicit JudgeCommunicator(QObject *parent = nullptr);

signals:

public slots:
    void setP1(QString name);
    void setP2(QString name);
    void sendMap(QString map);
    void sendResult(QString result);
    void sendBattleResult(int result);

private:
    QNetworkAccessManager *judgeServer;
    QUrl host;
    QString p1;
    QString p2;
};

#endif // JUDGECOMMUNICATOR_H
