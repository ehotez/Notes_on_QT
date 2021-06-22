#ifndef NOTES_H
#define NOTES_H

#include <QMainWindow>
#include <QFile>
#include <QMessageBox>
#include <QFileInfo>
#include <QDirIterator>
#include <QTranslator>
#include <QLibraryInfo>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Notes; }
QT_END_NAMESPACE

class Notes : public QMainWindow
{
    Q_OBJECT

public:
    Notes(QWidget *parent = nullptr);
    ~Notes();

private slots:


    void on_newNoteButton_clicked();

    void on_tabWidget_tabCloseRequested(int index);

    void on_new_note_triggered();

    void on_exit_triggered();

    void autoSave();

    void loadSave();

    void closeEvent(QCloseEvent *event);

    void on_save_triggered();

    void noteStatus();

    void on_add_triggered();

    void on_exportButton_clicked();

    void on_export_note_triggered();

    void on_pushButton_clicked();

private:
    Ui::Notes *ui;
    int index;
    int counter = 0;
};
#endif // NOTES_H
