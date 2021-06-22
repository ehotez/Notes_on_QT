#include "notes.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Notes note;
    note.setWindowTitle( "Заметки" );

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    note.show();
    return a.exec();
}
