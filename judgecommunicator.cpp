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
    mapinfo["token"] = "73f57cbb65d884edd9b1897b9d41d1c82dc7244a8c49478b0f2c5fbecda6838f";
    mapinfo["level"] = 1;
    mapinfo["data"] = QJsonValue(map);

    QString msg(QJsonDocument(mapinfo).toJson(QJsonDocument::Compact));

    QNetworkRequest req(host);

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setHeader(QNetworkRequest::ContentLengthHeader, msg.length());

    judgeServer->post(req, QByteArray(msg.toStdString().c_str()));
}
