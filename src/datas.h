#ifndef DATAS_H
#define DATAS_H

#include <QStringList>

struct QuestionData
{
    QString text;
    QStringList answeredVariants;
    QString id;
    bool correctAnswered = true;
};

struct PersonInfo
{
    QString name;
};

#endif // DATAS_H
