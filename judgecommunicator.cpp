#include "judgecommunicator.h"

#include <iostream>

JudgeCommunicator::JudgeCommunicator(QObject *parent) : QObject(parent),
    judgeServer(new QNetworkAccessManager(this)),
    host("http://hmkrl.com")
{
    host.setPort(3000);
    host.setPath("/game");
}

void JudgeCommunicator::sendMap(QString map)
{
    QJsonObject mapinfo;
    mapinfo["token"] = "f6ea5cc739cae0a852d04f95fcf9f3d8377d3079816e4f5ed397add46d0fad2a";
    mapinfo["level"] = 1;
    mapinfo["data"] = QJsonValue(map);

    QString msg(QJsonDocument(mapinfo).toJson(QJsonDocument::Compact));

    QNetworkRequest req(host);

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setHeader(QNetworkRequest::ContentLengthHeader, msg.length());

    judgeServer->post(req, QByteArray(msg.toStdString().c_str()));
}
