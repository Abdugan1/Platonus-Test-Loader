#include "networkaccessmanager.h"
#include "platonusviewerapp.h"

#include <QDebug>

PlatonusViewerApp::PlatonusViewerApp(QObject* parent)
    : QObject(parent)
    , networkAccessManager_(new NetworkAccessManager(this))
    , authorization_(networkAccessManager_)
    , platonusTestLoader_(networkAccessManager_)
{
    authorization_.show();


    connect(&authorization_, &Authorization::success, this, &PlatonusViewerApp::onAuthorizationSuccess);
    connect(&authorization_, &Authorization::success, &platonusTestLoader_, &PlatonusTestLoader::obtainTestingsInfo);

    connect(&platonusTestLoader_, &PlatonusTestLoader::logOuted, this, &PlatonusViewerApp::onLogOuted);
}

PlatonusViewerApp::~PlatonusViewerApp()
{
}

void PlatonusViewerApp::onAuthorizationSuccess()
{
    authorization_.close();
    platonusTestLoader_.show();
}

void PlatonusViewerApp::onLogOuted()
{
    platonusTestLoader_.close();
    authorization_.show();
}
