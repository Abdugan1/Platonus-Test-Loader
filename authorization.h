#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QWidget>

namespace Ui {
class Authorization;
}

class NetworkAccessManager;
class QNetworkReply;
class QRegularExpression;

class Authorization : public QWidget
{
    Q_OBJECT

public:
    explicit Authorization(NetworkAccessManager* networkAccessManager, QWidget *parent = nullptr);
    ~Authorization();

    struct AuthInfo
    {
        QString username;
        QString password;
    };
    AuthInfo authInfo;

signals:
    void success();
private slots:
    void authRequestFinished();
    void on_logInButton_clicked();
//    void saveAuthorizationParameters(); // will be soon
//    void loadAuthorizationParameters(); // will be soon :)
private:
    void sendAuthRequest(const QString& username, const QString& password);

    Ui::Authorization *ui;
    NetworkAccessManager* networkAccessManager_;
};

#endif // AUTHORIZATION_H
