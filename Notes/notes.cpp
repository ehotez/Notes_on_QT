#include "notes.h"
#include "ui_notes.h"
#include "add_note.h"

Notes::Notes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Notes)
{
    ui->setupUi(this);
    ui->tabWidget->clear();
    loadSave();
}

Notes::~Notes()
{
    delete ui;
}

//Реализация кнопки "Новая заметка"
void Notes::on_newNoteButton_clicked()
{
    counter++; //счетчик количества открытых заметок
    add_note *newNote = new add_note(this);
    QString name = "Заметка " + QString::number(ui->tabWidget->count() + 1);
    connect(newNote->text,
            SIGNAL(textChanged()), this, SLOT(noteStatus()));
    ui->tabWidget->addTab(newNote, name);
    index = ui->tabWidget->count() - 1;
    ui->tabWidget->setCurrentIndex(index);
    if(maxcounter < counter)
        maxcounter = counter;
}

void Notes::on_tabWidget_tabCloseRequested(int index)
{
    counter--;
    ui->tabWidget->removeTab(index);
    int i = counter;
    while (i > index){
        ui->tabWidget->setTabText(i-1, "Заметка " + QString::number(i));
        i--;
    }

}

void Notes::on_new_note_triggered()
{
    on_newNoteButton_clicked();
}

void Notes::closeEvent(QCloseEvent *event)
{
    event->ignore();
    qDebug()<<event;
    for(int i = 0; i < counter; i++){
        add_note *an =  qobject_cast<add_note*>
                (ui->tabWidget->widget(i));
        //При несохраненных данных при закрытии программы выводится предупреждение
        if(an->saved == false || maxcounter != counter){
            QMessageBox msgBox;
            msgBox.setWindowTitle("Закрытие программы");
            msgBox.setText("Остались несохраненные изменения. Сохранить?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            int ret = msgBox.exec();
            switch(ret){
            case QMessageBox::Yes:
                 autoSave();
                 event->accept();
                 break;
              case QMessageBox::No:
                 event->accept();
                 maxcounter = counter;
                 break;
              case QMessageBox::Cancel:
                 event->ignore();
                 break;
            }
            break;
        }else{event->accept();}
    }
}

//Функция сохранения заметок
void Notes::autoSave()
{
    //Проверяем на существование каталога с файлами сохранения
    if(!QDir("saves").exists())
        QDir().mkdir("saves");

    //Отчищаем каталог с сохранениями от предыдущих сохранений
    QString delDir = "saves/";
    QDirIterator it(delDir, QDirIterator::Subdirectories);

    while(it.hasNext()){
        QFileInfo fi(it.next());

        QString filename = fi.canonicalFilePath();
        if(fi.isFile())
            QFile(filename).remove();
    }

    //Сохраняем counter количество заметок
    for(int i = 0; i < counter; i++){
        QString filename = "saves/Заметка " + QString::number(i+1);
        ui->tabWidget->setTabText(i, "Заметка " + QString::number(i+1));
        QFile file(filename);
        file.open(QIODevice::WriteOnly);

        add_note *an =  qobject_cast<add_note*>
                (ui->tabWidget->widget(i));
        an->saved = true;
        maxcounter = counter;
        file.write(an->text->toPlainText().toUtf8());

        file.close();
    }
}

//Функция загрузки сохранений
void Notes::loadSave()
{
    int i = 0;
    QString saveDir = "saves/";
    QDirIterator it(saveDir, QDirIterator::Subdirectories);

    //Обходим все файл в каталоге с сохранениями
    while(it.hasNext()){
        QFileInfo fi(it.next());
        if(fi.isFile()){
            QString filename = fi.canonicalFilePath();
            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            //Создаем для каждого файла отдельную вкладку
            on_newNoteButton_clicked();
            add_note *an =  qobject_cast<add_note*>
                    (ui->tabWidget->widget(i));

            an->text->setPlainText(file.readAll());
            QFileInfo finfo(file); 
            ui->tabWidget->setTabText(i, finfo.fileName());
            an->saved = true;
            file.close();
            i++;
        }
    }
}

//Проверяем статус заметки и оповещаем пользователя символом "*"
void Notes::noteStatus()
{
    int index = ui->tabWidget->currentIndex();
    add_note *an =  qobject_cast<add_note*>
            (ui->tabWidget->widget(index));

    if(an->saved == true){
        QString noteName = ui->tabWidget->tabText(index);
        noteName = "*" + noteName;
        ui->tabWidget->setTabText(index, noteName);
        an->saved = false;
    }
}

void Notes::on_exit_triggered()
{
    close();
}

void Notes::on_save_triggered()
{
    autoSave();
    statusBar()->showMessage(tr("Сохранено"));
}

void Notes::on_add_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"Открыть");
    QFile file(filename);
    QFileInfo fi(filename);

    if(fi.isFile()){
        file.open(QIODevice::ReadOnly);
        on_newNoteButton_clicked();
        add_note *an =  qobject_cast<add_note*>
                (ui->tabWidget->widget(counter - 1));

        an->text->setPlainText(file.readAll());
        QFileInfo finfo(file);
        ui->tabWidget->setTabText(counter - 1, finfo.fileName());
        file.close();
        statusBar()->showMessage(tr("Файл добавлен"));
    }
}

void Notes::on_exportButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
                this,
                "Сохранить",
                tr("Безымянная заметка"),
                tr("*.txt"));
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    index = ui->tabWidget->currentIndex();

    add_note *an =  qobject_cast<add_note*>
            (ui->tabWidget->widget(index));

    file.write(an->text->toPlainText().toUtf8());

    file.close();
    statusBar()->showMessage(tr("Экспортировано"));
}

void Notes::on_export_note_triggered()
{
    on_exportButton_clicked();
}
