#ifndef DATAS_H
#define DATAS_H

#include <QStringList>

struct QuestionData
{
    QString text;
    QStringList variants;
    QString id;
    bool correctAnswered = true;
};

#endif // DATAS_H
