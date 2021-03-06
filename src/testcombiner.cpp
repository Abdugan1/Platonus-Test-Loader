#include "testcombiner.h"
#include "ui_testcombiner.h"
#include "questiondata.h"
#include "internal.h"

#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

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

    filePaths_ = QFileDialog::getOpenFileNames(this);
    for (const auto& filePath : filePaths_) {
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

QString TestCombiner::getFilesContent()
{
    QRegularExpression questionTextReg("#question#(.*?)<");
    QRegularExpression variantReg("#variant#(.*?)<");
    QRegularExpression fontColorReg("<font color=\\\"(.*?)\\\">");

    QList<QuestionData> questionDatas;
    for (const auto& filePath : filePaths_) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Warning"), file.errorString());
            return QString();
        }
        QTextStream in(&file);
        QString line = in.readLine();
        while (!in.atEnd()) {
            QuestionData questionData;
            questionData.text = Internal::getAllMatches(line, questionTextReg).first();
            line = in.readLine();
            while (!line.contains("#question#") && line.contains("#")) {
                QString variant = Internal::getAllMatches(line, variantReg).first();
                questionData.variants.append(variant);
                QString fontColor = Internal::getAllMatches(line, fontColorReg).first();
                questionData.correctAnswered = (fontColor == "green");
                line = in.readLine();
            }
            questionDatas.append(questionData);
        }

        file.close();
    }
    for (const auto& questionData : questionDatas) {
        qDebug() << "text:"<<questionData.text;
        qDebug() << "variants: " << questionData.variants;
        qDebug() << "CA:" << questionData.correctAnswered;
    }

    return QString();
}

void TestCombiner::on_combineButton_clicked()
{
    getFilesContent();
}
