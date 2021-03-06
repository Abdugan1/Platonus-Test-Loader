#include "testcombiner.h"
#include "ui_testcombiner.h"

#include <QFileDialog>
#include <QLabel>

TestCombiner::TestCombiner(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestCombiner)
{
    ui->setupUi(this);
    setFixedSize(this->size());
}

TestCombiner::~TestCombiner()
{
    delete ui;
}


void TestCombiner::on_openFilesButton_clicked()
{
    if (!ui->fileNamesLayout->isEmpty())
        deleteAllFileNames();

    QStringList filePaths = QFileDialog::getOpenFileNames(this);
    for (const auto& filePath : filePaths) {
        QString fileName = filePath.mid(filePath.lastIndexOf("/") + 1);
        QLabel* fileNameLabel = new QLabel(fileName);
        ui->fileNamesLayout->insertWidget(0, fileNameLabel);
    }
}

void TestCombiner::deleteAllFileNames()
{
    QLayoutItem *child;
    while ((ui->fileNamesLayout->count() != 1)
           && (child = ui->fileNamesLayout->takeAt(0)) != 0)
    {
        child->widget()->setParent(nullptr);
        delete child;
    }
}
