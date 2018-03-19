#include "judgecommunicator.h"

#include <QProcessEnvironment>
#include <QNetworkReply>
#include <QTimer>
#include <QEventLoop>

JudgeCommunicator::JudgeCommunicator(QObject *parent) : QObject(parent),
    resultReply(NULL),
    judgeServer(new QNetworkAccessManager(this)),
    host("http://localhost")
{
    host.setPort(3000);
    host.setPath("/game");
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

    resultReply = judgeServer->post(req, QByteArray(msg.toStdString().c_str()));
}

void JudgeCommunicator::waitResultFinished(int timeout)
{
    QEventLoop loop;
    if(resultReply == NULL) return;

    connect(resultReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QTimer::singleShot(timeout, &loop, &QEventLoop::quit);
    loop.exec();
}
