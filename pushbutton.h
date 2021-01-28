#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>

class PushButton : public QPushButton
{
    Q_OBJECT
public:
    PushButton(const QString& text, QWidget* parent = nullptr) : QPushButton(text, parent) {}
    struct TestingData
    {
        QString name;
        QString id;
    };
    TestingData testingData;
};

#endif // PUSHBUTTON_H
