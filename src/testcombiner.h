#ifndef TESTCOMBINER_H
#define TESTCOMBINER_H

#include <QWidget>

namespace Ui {
class TestCombiner;
}

class TestCombiner : public QWidget
{
    Q_OBJECT

public:
    explicit TestCombiner(QWidget *parent = nullptr);
    ~TestCombiner();

private slots:

    void on_openFilesButton_clicked();

private:
    void deleteAllFileNames();
    Ui::TestCombiner *ui;
};

#endif // TESTCOMBINER_H
