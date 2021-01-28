#include "networkaccessmanager.h"
#include "platonustestloader.h"
#include "ui_platonustestloader.h"
#include "internal.h"
#include "questiondata.h"

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextDocumentFragment>

PlatonusTestLoader::PlatonusTestLoader(NetworkAccessManager* networkAccessManager, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlatonusTestLoader),
    networkAccessManager_(networkAccessManager)
{
    ui->setupUi(this);

    ui->finishDateEdit->setDate(QDate::currentDate());
    setFixedSize(this->size());
}

PlatonusTestLoader::~PlatonusTestLoader()
{
    delete ui;
}

void PlatonusTestLoader::closeEvent(QCloseEvent* event)
{
    logOut();
}

void PlatonusTestLoader::obtainTestingsInfo()
{
    sendObtainTestingInfoRequest();
}

void PlatonusTestLoader::getTestingsInfoFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError)
        QMessageBox::warning(this, tr("Warning"), tr("Error while obtaining information about testings.\nTry logging in again."));
    else {
        QString content =  reply->readAll();
        getAllTestsData(content);
        showTestsButton();
    }

    reply->deleteLater();
}

void PlatonusTestLoader::onButtonClicked()
{
    QString buttonId= qobject_cast<PushButton*>(sender())->testingData.id;
    for (const auto& testingData: qAsConst(testingDataList_)) {
        if (buttonId == testingData.id) {
            loadTest(testingData);
            break;
        }
    }
}

void PlatonusTestLoader::loadTest(const TestingData& testingData)
{
    const QUrl testingUrl("https://edu2.aues.kz/rest/testing_student/testing/ru/" + testingData.id);
    QNetworkReply* reply = networkAccessManager_->sendGet(testingUrl);
    // wait untill reply is not finished
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QString content = reply->readAll();
    QString fileContent = getFinalContent(content, testingData);

    saveTest(fileContent, testingData);
}

//void PlatonusTestLoader::loadImages(const QString& str, int questionNum)
//{
//    // not done yer
//    QUrl url(QStringLiteral("https://edu2.aues.kz/getImage?id=409034"));

//    QRegularExpression imgIdReg("getImage\?id=(\\d+)");
//    QStringList imagesId = Internal::getAllMatches(str, imgIdReg);
//    int size = imagesId.size();
//    auto idIter = imagesId.begin();
//    for (int i = 0; i < size; ++i) {
//        QUrl url("https://edu2.aues.kz/getImage?id=" + *idIter++);
//    }
//}

void PlatonusTestLoader::saveTest(const QString& fileContent, const TestingData& testingData)
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save File","./" + testingData.name + ".txt");
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out.setGenerateByteOrderMark(true);
    out << fileContent.toUtf8();
    file.close();
}

void PlatonusTestLoader::getAllTestsData(const QString& str)
{
    const QRegularExpression testingNameRegex("class=\"filecabinetLink\">(.*)<\\/a>");
    const QRegularExpression testingIdRegex("testingID=(\\d+)");

    QStringList testingNames    = Internal::getAllMatches(str ,testingNameRegex);
    QStringList testingIds      = Internal::getAllMatches(str, testingIdRegex);

    auto testingIdsIter     = testingIds.begin();
    auto testingNamesIter   = testingNames.begin();

    if (!testingDataList_.isEmpty())
        testingDataList_.clear();

    for (int i = 0; i < testingNames.size(); ++i)
        testingDataList_.push_back({*testingNamesIter++, *testingIdsIter++});
}

void PlatonusTestLoader::showTestsButton()
{
    PushButton* button = nullptr;
    if (!ui->testingNamesLayout->isEmpty())
        deleteAllTestsButton();
    for (const auto& testingInfo : testingDataList_) {
        button = createButton(testingInfo);
        ui->testingNamesLayout->addWidget(button);
    }
}

void PlatonusTestLoader::setAllQuestionData(const QStringList& questionBlocks)
{
    static const QRegularExpression questionTextReg("\"questionText\\\":.*?\\\"(((?!<img src).)*?)\\\"");
    static const QRegularExpression questionIdReg("questionID\\\":(\\d+)");
    static const QRegularExpression answeredVariantReg(":\\\"(((?!value).)*)\\\",\\\"changed\\\":true");

    if (!questionDataList_.isEmpty())
        questionDataList_.clear();

    for (const QString& questionBlock : questionBlocks) {
        QString     questionText        = Internal::getAllMatches(questionBlock, questionTextReg).first();
        QString     questionId          = Internal::getAllMatches(questionBlock, questionIdReg).first();
        QStringList answeredVariants    = Internal::getAllMatches(questionBlock, answeredVariantReg);

        QuestionData questionData;
        questionData.text       = questionText;
        questionData.id         = questionId;
        questionData.variants   = answeredVariants;

        questionDataList_.push_back(questionData);
    }
}

QStringList PlatonusTestLoader::getIncorrectAnswered(const TestingData& testingData)
{
    const QUrl url("https://edu2.aues.kz/student_appeals?option=view&"
                    "testingID=" + testingData.id +"&countInPart=30startDate=nullfinishDate=null&"
                    "nocache=&1606359543&start_date=26-10-2020&2021-01-26=26-01-2021&page=0");
    QNetworkReply* reply = networkAccessManager_->sendGet(url);
    // wait untill reply is not finished
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QString content = reply->readAll();

    static const QRegularExpression questionIdReg("id=\"q_(\\d+)\"");
    QStringList incorrectAnswered = Internal::getAllMatches(content, questionIdReg);
    return incorrectAnswered;
}

void PlatonusTestLoader::highlightIncorrect(const QStringList& incorrectAnswered)
{
    for (auto& questionData : questionDataList_) {
        if (!incorrectAnswered.contains(questionData.id))
            continue;
        questionData.variants.first().insert(0, "#___!WRONG!___#    ");
    }
}

QString PlatonusTestLoader::getFinalContent(const QString& allContent, const TestingData& testingData)
{
    static const QRegularExpression questionBlockRegex("{\"questionType\".*?\"variants\":\\[{.*?}\\].*?}");
    QStringList questionBlocks = Internal::getAllMatches(allContent, questionBlockRegex, 0);
    setAllQuestionData(questionBlocks);
    QStringList incorrectAnswered = getIncorrectAnswered(testingData);
    highlightIncorrect(incorrectAnswered);

    int i = 0;
    QString fileContent;
    for (const auto& questionData : questionDataList_) {
        QString text = QTextDocumentFragment::fromHtml(questionData.text).toPlainText();
        fileContent += QString::number(++i) + ") " + text + "\r";

        if (questionData.variants.isEmpty()) {
            fileContent += "\t#didn't answer#\r";
            continue;
        }

        auto begin  = questionData.variants.begin();
        auto end    = questionData.variants.end();
        for (auto it = begin; it != end; ++it) {
            QString variant = QTextDocumentFragment::fromHtml(*it).toPlainText();
            fileContent += "\t" + variant + "\r";
        }

    }
    return fileContent;
}

PushButton* PlatonusTestLoader::createButton(const TestingData& testingData)
{
    PushButton* button = new PushButton(testingData.name);
    button->testingData = testingData;
    connect(button, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
    return button;
}

void PlatonusTestLoader::sendObtainTestingInfoRequest()
{
    const QUrl testingPageUrl("https://edu2.aues.kz/student_appeals");
    QNetworkRequest postRequest(testingPageUrl);
    postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString startDate   = ui->startDateEdit->date().toString("dd-MM-yyyy");
    QString finishDate  = ui->finishDateEdit->date().toString("dd-MM-yyyy");

    QString postDataStr = "search=&start_date=" + startDate +"&finish_date=" + finishDate;

    QByteArray data(postDataStr.toStdString().c_str());

    QNetworkReply* postReply = networkAccessManager_->post(postRequest, data);
    connect(postReply, SIGNAL(finished()), this, SLOT(getTestingsInfoFinished()));
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

void PlatonusTestLoader::logOut()
{
    const QUrl logoutUrl(QStringLiteral("https://edu2.aues.kz/rest/api/logout/"));
    QNetworkRequest logoutRequest(logoutUrl);
    logoutRequest.setRawHeader("token", networkAccessManager_->token().toUtf8());

    networkAccessManager_->post(logoutRequest, QByteArray());
}

void PlatonusTestLoader::on_logOutButton_clicked()
{
    logOut();
    emit logOuted();
}

void PlatonusTestLoader::on_finishDateEdit_userDateChanged(const QDate &date)
{
    deleteAllTestsButton();
    obtainTestingsInfo();
}

void PlatonusTestLoader::on_startDateEdit_userDateChanged(const QDate &date)
{
    deleteAllTestsButton();
    obtainTestingsInfo();
}
