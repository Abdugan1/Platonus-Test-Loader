#ifndef PLATONUSTESTLOADERAPP_H
#define PLATONUSTESTLOADERAPP_H

#include "authorization.h"

#include <QObject>

class NetworkAccessManager;

class PlatonusTestLoaderApp : public QObject
{
    Q_OBJECT
public:
    explicit PlatonusTestLoaderApp(QObject *parent = nullptr);

signals:
private slots:
    void onSuccessAuthorization();
private:
    NetworkAccessManager* networkCtrl_;

    Authorization authorization_;
};

#endif // PLATONUSTESTLOADERAPP_H
