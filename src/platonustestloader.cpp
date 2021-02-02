#include "platonustestloader.h"
#include "ui_platonustestloader.h"
#include "networkaccessmanager.h"
#include "internal.h"
#include "datas.h"

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

void PlatonusTestLoader::onTestButtonClicked()
{
    TestData testData = qobject_cast<TestButton*>(sender())->testData;
    loadTest(testData);
}

void PlatonusTestLoader::sendAppealsRequest()
{
    static const QUrl testingPageUrl("https://edu2.aues.kz/student_appeals");
    QNetworkRequest postRequest(testingPageUrl);
    postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString startDate   = ui->startDateEdit->date().toString("dd-MM-yyyy");
    QString finishDate  = ui->finishDateEdit->date().toString("dd-MM-yyyy");

    QString postDataStr = "search=&start_date=" + startDate + "&finish_date=" + finishDate;

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
    connect(button, SIGNAL(clicked()), this, SLOT(onTestButtonClicked()));
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

void PlatonusTestLoader::loadTest(const TestData& testData)
{
    const QUrl testingUrl("https://edu2.aues.kz/rest/testing_student/testing/ru/" + testData.id);
    networkCtrl_->sendGet(testingUrl);
    QString content = networkCtrl_->content();
    static const QRegularExpression questionBlockRegex("{\"questionType\".*?"
                                                        "\"variants\":\\[{.*?}\\].*?}");
    QStringList questionBlocks = Internal::getAllMatches(content, questionBlockRegex, 0);
    QList<QuestionData> questionDataList = getQuestionsData(questionBlocks);

    int i = 0;
    for (const auto& questionData : questionDataList) {
        qDebug() << ++i;
        qDebug() << "Text:\n" << questionData.text;
        qDebug() << "id:\n" << questionData.id;
        qDebug() << "variants:\n" << questionData.variants;
        qDebug() << "================================================================";
    }

}

QList<QuestionData> PlatonusTestLoader::getQuestionsData(const QStringList& questionBlocks)
{
    QList<QuestionData> questionDataList;

    static const QRegularExpression questionTextReg("\\\"questionText\\\":\\\"(.*)\\\",\\\"number");
    static const QRegularExpression questionIdReg("questionID\\\":(\\d+)");
    static const QRegularExpression answeredVariantReg(":\\\"(((?!value).)*)\\\",\\\"changed\\\":true");

    static auto deleteJunk = [](QString& str) {
        static const QRegularExpression junkReg("<<(((?!img).)*)>>");
        str = str.remove(junkReg);
    };

    for (const auto& questionBlock : questionBlocks) {
        QString     questionText        = Internal::getAllMatches(questionBlock, questionTextReg).first();
        QString     questionId          = Internal::getAllMatches(questionBlock, questionIdReg).first();
        QStringList answeredVariants    = Internal::getAllMatches(questionBlock, answeredVariantReg);

        // deleting all junks, leaving only necessary
        static const QRegularExpression junkReg("<<(((?!img).)*)>>");
        deleteJunk(questionText);
        for (auto& variant : answeredVariants)
            deleteJunk(variant);

        QuestionData questionData;
        questionData.text       = questionText;
        questionData.id         = questionId;
        questionData.variants   = answeredVariants;

        questionDataList.push_front(questionData);
    }

    return questionDataList;
}

void PlatonusTestLoader::on_startDateEdit_userDateChanged(const QDate &date)
{
    showTestButtons();
}

void PlatonusTestLoader::on_finishDateEdit_userDateChanged(const QDate &date)
{
    showTestButtons();
}
