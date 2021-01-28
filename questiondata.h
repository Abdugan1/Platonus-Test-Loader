#ifndef QUESTIONDATA_H
#define QUESTIONDATA_H

#include <QStringList>

struct QuestionData
{
    QString id;
    QString text;
    QStringList variants;
};

#endif // QUESTIONDATA_H
