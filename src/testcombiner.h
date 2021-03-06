#ifndef TESTCOMBINER_H
#define TESTCOMBINER_H

#include <QWidget>

namespace Ui {
class TestCombiner;
}

struct QuestionData;

class TestCombiner : public QWidget
{
    Q_OBJECT

public:
    explicit TestCombiner(QWidget *parent = nullptr);
    ~TestCombiner();

private slots:

    void on_openFilesButton_clicked();

    void on_combineButton_clicked();

private:
    void deleteAllFileNames();
    QString getFilesContent();
    Ui::TestCombiner *ui;
    QStringList filePaths_;
};

#endif // TESTCOMBINER_H
