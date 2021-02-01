#ifndef PLATONUSTESTLOADER_H
#define PLATONUSTESTLOADER_H

#include <QMainWindow>

namespace Ui {
class PlatonusTestLoader;
}

class NetworkAccessManager;

class PlatonusTestLoader : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlatonusTestLoader(NetworkAccessManager* networkCtrl, QWidget *parent = nullptr);
    ~PlatonusTestLoader();
signals:
    void logOuted();
public slots:
    void obtainTestsData();
private slots:
    void logOut();
    void on_logOutButton_clicked();

private:

    Ui::PlatonusTestLoader *ui;
    NetworkAccessManager* networkCtrl_;
};

#endif // PLATONUSTESTLOADER_H
