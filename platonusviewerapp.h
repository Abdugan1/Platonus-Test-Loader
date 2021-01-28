#ifndef PLATONUSMARKSVIEWER_H
#define PLATONUSMARKSVIEWER_H

#include "authorization.h"
#include "platonustestloader.h"

class NetworkAccessManager;
struct Subject;

#include <QObject>

class PlatonusViewerApp : public QObject
{
    Q_OBJECT
public:
    PlatonusViewerApp(QObject* parent = nullptr);
    ~PlatonusViewerApp();
private slots:
    void onAuthorizationSuccess();
    void onLogOuted();
private:
    NetworkAccessManager* networkAccessManager_;

    Authorization authorization_;
    PlatonusTestLoader platonusTestLoader_;
};

#endif // PLATONUSMARKSVIEWER_H
