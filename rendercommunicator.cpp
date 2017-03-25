#include "rendercommunicator.h"

#include <unistd.h>

RenderCommunicator::RenderCommunicator(QObject *parent) : QObject(parent),
    renderServer(new QNetworkAccessManager(this)),
    hostAddress("http://localhost"),
    port(3001)
{

}

void RenderCommunicator::setP1(QString name)
{
    p1 = name;
}

void RenderCommunicator::setP2(QString name)
{
    p2 = name;
}

void RenderCommunicator::addCurrentMinion(QJsonObject m)
{
    current_minion.append(m);
}

void RenderCommunicator::addNewMinion(QJsonObject m)
{
    new_minion.append(m);
}

void RenderCommunicator::addBuilding(QJsonObject b)
{
    buildings.append(b);
}

void RenderCommunicator::clear()
{
    while(!current_minion.empty()) current_minion.pop_back();
    while(!new_minion.empty()) new_minion.pop_back();
    while(!buildings.empty()) buildings.pop_back();
}

void RenderCommunicator::sendMap()
{
    /* pack json command */
    cmd["p1"] = p1;
    cmd["p2"] = p2;
    cmd["cmd"] = "battle";
    cmd["current_minion"] = current_minion;
    cmd["new_minion"] = new_minion;
    cmd["buildings"] = buildings;

    /* send command */
    QUrl host(hostAddress);
    QUrlQuery qry;

    host.setPort(port);
    host.setPath("/game_cmd");
    qry.addQueryItem("p1", p1);
    qry.addQueryItem("p2", p2);
    host.setQuery(qry);

    QString msg(QJsonDocument(cmd).toJson(QJsonDocument::Compact));

    QNetworkRequest req(host);

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setHeader(QNetworkRequest::ContentLengthHeader, msg.length());

    renderServer->post(req, QByteArray(msg.toStdString().c_str()));

    clear();
}

void RenderCommunicator::sendEnd(int winner)
{
    QUrl host(hostAddress);
    QUrlQuery qry;
    QJsonObject end_package;

    end_package["p1"] = p1;
    end_package["p2"] = p2;

    if(winner == 1) {
        end_package["winner"] = p1;
    }
    else {
        end_package["winner"] = p2;
    }

    host.setPort(port);
    host.setPath("/game_end");
    qry.addQueryItem("p1", p1);
    qry.addQueryItem("p2", p2);
    host.setQuery(qry);

    QString msg(QJsonDocument(end_package).toJson(QJsonDocument::Compact));

    QNetworkRequest req(host);

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setHeader(QNetworkRequest::ContentLengthHeader, msg.length());

    renderServer->post(req, QByteArray(msg.toStdString().c_str()));
    sleep(1);
}
