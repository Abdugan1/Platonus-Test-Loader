#include "platonustestloaderapp.h"
#include "networkaccessmanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//    PlatonusTestLoaderApp platonusTestLoaderApp;
    TestCombiner testCombiner;
    testCombiner.show();

    return app.exec();
}
