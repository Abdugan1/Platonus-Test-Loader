#include "platonustestloader.h"
#include "ui_platonustestloader.h"
#include "networkaccessmanager.h"

#include <QMessageBox>

PlatonusTestLoader::PlatonusTestLoader(NetworkAccessManager* networkCtrl, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlatonusTestLoader)
    , networkCtrl_(networkCtrl)

{
    ui->setupUi(this);
    ui->finishDateEdit->setDate(QDate::currentDate());
    setFixedSize(this->size());
}

PlatonusTestLoader::~PlatonusTestLoader()
{
    delete ui;
}

void PlatonusTestLoader::obtainTestsData()
{
    static const QUrl testsPageUrl("https://edu2.aues.kz/student_appeals");

    QString startDate   = ui->startDateEdit->date().toString("dd-MM-yyyy");
    QString finishDate  = ui->finishDateEdit->date().toString("dd-MM-yyyy");

    QString postData = "search=&start_date=" + startDate +"&finish_date=" + finishDate;

    networkCtrl_->sendPost(testsPageUrl, postData);

    if (networkCtrl_->errorStatus() != ErrorStatus::NoError) {
        QMessageBox::warning(this, tr("Warning")
                             , tr("Error while obtaining information about testings.\n"
                                  "Try logging in again."));
        return;
    }

    QString content = networkCtrl_->content();
    qDebug() << content;
}

void PlatonusTestLoader::logOut()
{
    static const QUrl logoutUrl(QStringLiteral("https://edu2.aues.kz/rest/api/logout/"));
    networkCtrl_->sendPost(logoutUrl, "");
    emit logOuted();
}

void PlatonusTestLoader::on_logOutButton_clicked()
{
    logOut();
}
