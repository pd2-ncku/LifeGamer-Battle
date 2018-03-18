#include "judgecommunicator.h"

#include <QProcessEnvironment>

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
    mapinfo["token"] = QProcessEnvironment::systemEnvironment().value("TOKEN");
    mapinfo["level"] = QProcessEnvironment::systemEnvironment().value("LEVEL");
    mapinfo["data"] = QJsonValue(map);

    QString msg(QJsonDocument(mapinfo).toJson(QJsonDocument::Compact));

    QNetworkRequest req(host);

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setHeader(QNetworkRequest::ContentLengthHeader, msg.length());

    judgeServer->post(req, QByteArray(msg.toStdString().c_str()));
}
