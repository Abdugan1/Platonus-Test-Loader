#ifndef PLATONUSTESTLOADER_H
#define PLATONUSTESTLOADER_H

#include "testbutton.h"

#include <QMainWindow>

namespace Ui {
class PlatonusTestLoader;
}

class NetworkAccessManager;
struct QuestionData;

typedef TestButton::TestData TestData;

class PlatonusTestLoader : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlatonusTestLoader(NetworkAccessManager* networkCtrl, QWidget *parent = nullptr);
    ~PlatonusTestLoader();
signals:
    void logOuted();
public slots:
    void showTestButtons();
private slots:
    void logOut();
    void on_logOutButton_clicked();
    void on_startDateEdit_userDateChanged(const QDate &date);
    void on_finishDateEdit_userDateChanged(const QDate &date);
    void onTestButtonClicked();

private:
    void sendAppealsRequest();
    QList<TestData> getTestsData(const QString& replyContent);
    void setTestsButton(const QList<TestData>& testDataList);
    TestButton* createButton(const TestData& testData);
    void deleteAllTestsButton();
    void loadTest(const TestData& testData);
    QList<QuestionData> getQuestionsData(const QStringList& questionBlocks);

    Ui::PlatonusTestLoader *ui;
    NetworkAccessManager* networkCtrl_;
};

#endif // PLATONUSTESTLOADER_H
