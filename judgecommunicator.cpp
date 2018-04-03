#include "judgecommunicator.h"

#include <QProcessEnvironment>
#include <QNetworkReply>
#include <QTimer>
#include <QEventLoop>

JudgeCommunicator::JudgeCommunicator(QObject *parent) : QObject(parent),
    judgeServer(new QNetworkAccessManager(this)),
    host("http://localhost")
{
    host.setPort(3000);
    host.setPath("/game");
}

void JudgeCommunicator::setP1(QString name)
{
    p1 = name;
}

void JudgeCommunicator::setP2(QString name)
{
    p2 = name;
}

void JudgeCommunicator::sendMap(QString map)
{
    QJsonObject mapinfo;
    mapinfo["token"] = QProcessEnvironment::systemEnvironment().value("TOKEN");
    mapinfo["level"] = QProcessEnvironment::systemEnvironment().value("LEVEL");
    mapinfo["data"] = QJsonValue(map);

    QString msg(QJsonDocument(mapinfo).toJson(QJsonDocument::Compact));

    QNetworkRequest req(host);

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setHeader(QNetworkRequest::ContentLengthHeader, msg.length());

    judgeServer->post(req, QByteArray(msg.toStdString().c_str()));
}

void JudgeCommunicator::sendResult(QString result)
{
    QJsonObject judgeResult;
    judgeResult["token"] = QProcessEnvironment::systemEnvironment().value("TOKEN");
    judgeResult["level"] = QProcessEnvironment::systemEnvironment().value("LEVEL");
    judgeResult["data"] = QJsonValue(result);

    QString msg(QJsonDocument(judgeResult).toJson(QJsonDocument::Compact));

    QNetworkRequest req(host);

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setHeader(QNetworkRequest::ContentLengthHeader, msg.length());

    QNetworkReply *rep = judgeServer->post(req, QByteArray(msg.toStdString().c_str()));

    QEventLoop loop;
    connect(rep, SIGNAL(finished()), &loop, SLOT(quit()));

    loop.exec();
}

void JudgeCommunicator::sendBattleResult(int result)
{
    QJsonObject battleResult;
    battleResult["level"] = QJsonValue("battle");

    battleResult["p1"] = p1;
    battleResult["p2"] = p2;

    battleResult["result"] = QJsonValue(result);

    QString msg(QJsonDocument(battleResult).toJson(QJsonDocument::Compact));

    QNetworkRequest req(host);

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setHeader(QNetworkRequest::ContentLengthHeader, msg.length());

    QNetworkReply *rep = judgeServer->post(req, QByteArray(msg.toStdString().c_str()));

    QEventLoop loop;
    connect(rep, SIGNAL(finished()), &loop, SLOT(quit()));

    loop.exec();
}
