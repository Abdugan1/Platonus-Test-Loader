#include "networkaccessmanager.h"

#include <QEventLoop>

NetworkAccessManager::NetworkAccessManager(QObject* parent)
    : QNetworkAccessManager(parent)
{

}

QString NetworkAccessManager::sendGet(const QUrl& url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("token", token_.toUtf8());

    QNetworkReply* reply = QNetworkAccessManager::get(request);
    waitUntillFinished(reply);
    setErrorStatus(reply);
    return reply->readAll();
}

QString NetworkAccessManager::sendPost(const QUrl& url, const QString& postDataStr)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    if (!token_.isEmpty())
        request.setRawHeader("token", token_.toUtf8());
    QByteArray data(postDataStr.toStdString().c_str());

    QNetworkReply* reply = QNetworkAccessManager::post(request, data);
    waitUntillFinished(reply);
    setErrorStatus(reply);
    return reply->readAll();
}

const QString& NetworkAccessManager::token() const
{
    return token_;
}

void NetworkAccessManager::setToken(const QString& tok)
{
    token_ = tok;
}

void NetworkAccessManager::waitUntillFinished(const QNetworkReply* reply)
{
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void NetworkAccessManager::setErrorStatus(const QNetworkReply* reply)
{
    errorStatus_ = reply->error();
}
