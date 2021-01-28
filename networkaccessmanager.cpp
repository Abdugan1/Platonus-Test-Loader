#include "networkaccessmanager.h"

NetworkAccessManager::NetworkAccessManager(QObject* parent) :
    QNetworkAccessManager(parent)
{

}

void NetworkAccessManager::sendGet(QObject* sender, const QUrl& url, const char* method)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString authTok = token_;
    request.setRawHeader("token", token_.toUtf8());

    QNetworkReply* reply = QNetworkAccessManager::get(request);

    connect(reply , SIGNAL(finished()), sender, method);
}

QNetworkReply* NetworkAccessManager::sendGet(const QUrl& url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString authTok = token_;
    request.setRawHeader("token", token_.toUtf8());

    return QNetworkAccessManager::get(request);
}
