#include "platonustestloaderapp.h"
#include "networkaccessmanager.h"

PlatonusTestLoaderApp::PlatonusTestLoaderApp(QObject* parent)
    : QObject(parent)
    , networkCtrl_(new NetworkAccessManager(this))
    , authorization_(networkCtrl_)
{
    connect(&authorization_, SIGNAL(success()), this, SLOT(onSuccessAuthorization()));

    authorization_.show();
}

void PlatonusTestLoaderApp::onSuccessAuthorization()
{
    qDebug() << "success auth";
//    authorization_.close();
}
