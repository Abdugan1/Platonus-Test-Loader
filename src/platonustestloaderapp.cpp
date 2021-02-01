#include "platonustestloaderapp.h"
#include "networkaccessmanager.h"

PlatonusTestLoaderApp::PlatonusTestLoaderApp(QObject* parent)
    : QObject(parent)
    , networkCtrl_(new NetworkAccessManager(this))
    , authorization_(networkCtrl_)
    , platonusTestLoader_(networkCtrl_)
{
    connect(&authorization_, SIGNAL(success()), this, SLOT(onSuccessAuthorization()));
    connect(&platonusTestLoader_, SIGNAL(logOuted()), this, SLOT(onLogOut()));

    authorization_.show();
}

void PlatonusTestLoaderApp::onSuccessAuthorization()
{
    authorization_.close();
    platonusTestLoader_.show();
}

void PlatonusTestLoaderApp::onLogOut()
{
    platonusTestLoader_.close();
    authorization_.show();
}
