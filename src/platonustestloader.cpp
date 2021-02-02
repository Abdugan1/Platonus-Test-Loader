#include "platonustestloader.h"
#include "ui_platonustestloader.h"
#include "networkaccessmanager.h"
#include "internal.h"

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

void PlatonusTestLoader::showTestButtons()
{
    sendAppealsRequest();
    QString content = networkCtrl_->content();

    QList<TestData> testDataList = getTestsData(content);
    setTestsButton(testDataList);
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

void PlatonusTestLoader::sendAppealsRequest()
{
    static const QUrl testingPageUrl("https://edu2.aues.kz/student_appeals");
    QNetworkRequest postRequest(testingPageUrl);
    postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString startDate   = ui->startDateEdit->date().toString("dd-MM-yyyy");
    QString finishDate  = ui->finishDateEdit->date().toString("dd-MM-yyyy");

    QString postDataStr = "search=&start_date=" + startDate +"&finish_date=" + finishDate;

    networkCtrl_->sendPost(postRequest, postDataStr);
}

QList<TestData> PlatonusTestLoader::getTestsData(const QString& replyContent)
{
    QList<TestData> testDataList;

    static const QRegularExpression testingNameRegex("class=\"filecabinetLink\">(.*)<\\/a>");
    static const QRegularExpression testingIdRegex("testingID=(\\d+)");

    QStringList testingNames    = Internal::getAllMatches(replyContent ,testingNameRegex);
    QStringList testingIds      = Internal::getAllMatches(replyContent, testingIdRegex);

    auto testingIdsIter     = testingIds.begin();
    auto testingNamesIter   = testingNames.begin();

    for (int i = 0; i < testingNames.size(); ++i)
        testDataList.push_back({*testingNamesIter++, *testingIdsIter++});

    int i = 0;
    for (const auto& testData : testDataList)
        qDebug() << ++i << "Name:" << testData.name << "id:" << testData.id;

    return testDataList;
}

void PlatonusTestLoader::setTestsButton(const QList<TestData>& testDataList)
{
    if (!ui->testingNamesLayout->isEmpty())
        deleteAllTestsButton();

    TestButton* button = nullptr;
    for (const auto& testData : testDataList) {
        button = createButton(testData);
        ui->testingNamesLayout->addWidget(button);
    }
}

TestButton* PlatonusTestLoader::createButton(const TestData& testData)
{
    TestButton* button = new TestButton(testData.name);
    button->testData = testData;
    return button;
}

void PlatonusTestLoader::deleteAllTestsButton()
{
    QLayoutItem *child;
    while ((child = ui->testingNamesLayout->takeAt(0)) != 0)
    {
        child->widget()->setParent(nullptr);
        delete child;
    }
}

void PlatonusTestLoader::on_startDateEdit_userDateChanged(const QDate &date)
{
    showTestButtons();
}

void PlatonusTestLoader::on_finishDateEdit_userDateChanged(const QDate &date)
{
    showTestButtons();
}
