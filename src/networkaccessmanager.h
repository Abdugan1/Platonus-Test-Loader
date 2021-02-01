#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

class NetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    NetworkAccessManager(QObject* parent = nullptr);

    QString sendGet(const QUrl& url);
    QString sendPost(const QUrl& url, const QString& postDataStr);

    const QString& token() const;
    void setToken(const QString& tok);

private:
    void waitUntillFinished(const QNetworkReply* reply);
    void setErrorStatus(const QNetworkReply* reply);

    QString token_;
    QNetworkReply::NetworkError errorStatus_;
};

#endif // NETWORKACCESSMANAGER_H
