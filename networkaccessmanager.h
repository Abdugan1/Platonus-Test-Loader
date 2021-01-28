#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class NetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit NetworkAccessManager(QObject* parent = nullptr);

    void sendGet(QObject* sender, const QUrl& url, const char* method);
    QNetworkReply* sendGet(const QUrl& url);
    inline const QString& token() const { return token_; }
    inline void setToken(const QString& tok) { token_ = tok; }
private:
    QString token_;
};

#endif // NETWORKACCESSMANAGER_H
