#ifndef ADD_NOTE_H
#define ADD_NOTE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPlainTextEdit>

class add_note : public QWidget
{
    Q_OBJECT
public:
    explicit add_note(QWidget *parent = nullptr);
    QPlainTextEdit *text;
    bool saved = true;
private:
    QVBoxLayout *box;
};

#endif // ADD_NOTE_H
