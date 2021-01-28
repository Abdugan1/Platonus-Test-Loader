#include "platonusviewerapp.h"

#include <QApplication>
#include <QErrorMessage>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlatonusViewerApp platonusViewerApp;

    return a.exec();
}
