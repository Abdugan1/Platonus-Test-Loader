#ifndef PLATONUSTESTLOADER_H
#define PLATONUSTESTLOADER_H

#include "pushbutton.h"

#include <QMainWindow>

namespace Ui {
class PlatonusTestLoader;
}

class NetworkAccessManager;
class QuestionData;

class PlatonusTestLoader : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlatonusTestLoader(NetworkAccessManager* networkAccessManager, QWidget *parent = nullptr);
    ~PlatonusTestLoader();
protected:
    virtual void closeEvent(QCloseEvent* event) override;

signals:
    void logOuted();

public slots:
    void obtainTestingsInfo();
private slots:
    void getTestingsInfoFinished();
    void onButtonClicked();
    void on_logOutButton_clicked();
    void on_finishDateEdit_userDateChanged(const QDate& date);
    void on_startDateEdit_userDateChanged(const QDate& date);

private:
    typedef PushButton::TestingData TestingData;

    void loadTest(const TestingData& testingData);
//    void loadImages(const QString& str, int questionNum);   // not done yet
    void saveTest(const QString& fileContent, const TestingData& testingData);
private:
    void getAllTestsData(const QString& str);
    void showTestsButton();
    void setAllQuestionData(const QStringList& questionBlocks);
    QStringList getIncorrectAnswered(const TestingData& testingData);
    void highlightIncorrect(const QStringList& incorrectAnswered);
    QString getFinalContent(const QString& allContent, const TestingData& testingData);
    PushButton* createButton(const TestingData& testingData);
    void sendObtainTestingInfoRequest();
    void deleteAllTestsButton();

    void logOut();

    Ui::PlatonusTestLoader *ui;
    NetworkAccessManager* networkAccessManager_;
    QList<TestingData>  testingDataList_;
    QList<QuestionData> questionDataList_;
};

#endif // PLATONUSTESTLOADER_H
