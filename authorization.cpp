#include "authorization.h"
#include "ui_authorization.h"
#include "networkaccessmanager.h"

#include "internal.h"

#include <QMessageBox>

Authorization::Authorization(NetworkAccessManager* networkAccessManager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Authorization),
    networkAccessManager_(networkAccessManager)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint
                   | Qt::WindowCloseButtonHint);
    setFixedSize(this->size());
}

Authorization::~Authorization()
{
    delete ui;
}

void Authorization::on_logInButton_clicked()
{
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    // if username or password was not filled, then borders will be red
    if (username.isEmpty() || password.isEmpty()) {
        if (username.isEmpty())
            ui->usernameEdit->setStyleSheet("border: 1px solid red");
        if (password.isEmpty())
            ui->passwordEdit->setStyleSheet("border: 1px solid red");
        return;
    }

    sendAuthRequest(username, password);
}

void Authorization::authRequestFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError)
        QMessageBox::warning(this, tr("Authorization failed"), tr("Invalid password or username"));
    else {
        static const QRegularExpression tokenReg("\"auth_token\":\"(.*?)\"");
        QString content = reply->readAll();
        QString token =  Internal::getAllMatches(content, tokenReg).first();
        networkAccessManager_->setToken(token);
        emit success();
    }

    reply->deleteLater();
}

void Authorization::sendAuthRequest(const QString& username, const QString& password)
{
    authInfo.username = username;
    authInfo.password = password;

    static const QUrl logInUrl(QStringLiteral("https://edu2.aues.kz/rest/api/login"));
    QString postDataStr = "{\"login\":\"" + username + "\",\"iin\":null,\"icNumber\":null,\"password\":\"" + password + "\"}";

    QNetworkRequest postRequest(logInUrl);
    postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray data(postDataStr.toStdString().c_str());

    QNetworkReply* postReply = networkAccessManager_->post(postRequest, data);
    connect(postReply, SIGNAL(finished()), this, SLOT(authRequestFinished()));
}
