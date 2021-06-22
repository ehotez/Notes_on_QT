#include "add_note.h"

add_note::add_note(QWidget *parent) : QWidget(parent)
{
    box = new QVBoxLayout(this);
    text = new QPlainTextEdit(this);
    box->addWidget(text);
}
