#include "authorization.h"
#include "networkaccessmanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Authorization authorization(new NetworkAccessManager);
    authorization.show();

    return app.exec();
}
