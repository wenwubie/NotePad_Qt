#include "MainWindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QStringList>
#include <QMessageBox>
#include <QTextStream>
#include <QMap>
#include <QCloseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QList>
#include <QUrl>

QString MainWindow::showFileDialog(QFileDialog::AcceptMode mode, QString title)
{
    QString ret = "";
    QFileDialog dlg;
    QStringList filter;
    const char* filterArray[][2] =
    {
        {"Text File (*.txt)", ".txt"},
        {"All File(*)",                  "*"},
        {NULL,                       NULL}
    };

    QMap<QString, QString> map;

    for(int i=0; filterArray[i][0] != NULL; i++)
    {
        filter.append (filterArray[i][0]);
        map.insert (filterArray[i][0], filterArray[i][1]);
    }

    dlg.setAcceptMode (mode);
    dlg.setWindowTitle (title);
    dlg.setFilters (filter);

    if( mode == QFileDialog::AcceptOpen )
    {
        dlg.setFileMode (QFileDialog::ExistingFile);
    }

    if( dlg.exec () == QFileDialog::Accepted )
    {
        ret = dlg.selectedFiles ()[0];
        if( mode == QFileDialog::AcceptSave )
        {
            QString postfix = map[dlg.selectedFilter ()];

            if( (postfix != "*") && !ret.endsWith (postfix) )
            {
                ret += postfix;
            }
        }

    }

    return ret;
}

int MainWindow::showQueryMessage(QString msg)
{
    QMessageBox box;
    box.setWindowTitle("Query");
    box.setText (msg);
    box.setStandardButtons (QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel);

    return box.exec ();
}

void MainWindow::showErrorMessage(QString msg)
{
    QMessageBox box;
    box.critical (this,"Error",msg,QMessageBox::Ok);
}


QString MainWindow::saveCurrentData(QString path)
{
    QString ret = path;

    if( ret == "")
    {
        ret = showFileDialog (QFileDialog::AcceptSave, "Save File");
    }

    if( ret != "" )
    {
        QFile file(ret);

        if( file.open (QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream out(&file);

            out << mainEdit.toPlainText ();

            file.close ();

            setWindowTitle (QString("Notepad - ") + "[" + ret + "]" );

            isTextChanged = false;
        }

    }
    else
    {
        showErrorMessage ("save file error");

        ret = "";
    }

    return ret;
}

void MainWindow::preEditTextChanged()
{
    if( isTextChanged )
    {
        int choice = showQueryMessage("Do you want to save the changes?");

        switch(choice)
        {
            case QMessageBox::Ok:
                saveCurrentData (m_filePath);
                break;
            case QMessageBox::No:
                isTextChanged = false;
                break;
            case QMessageBox::Cancel:
                break;
            default:
                break;
        }
    }
}

void MainWindow::onFileNew()
{
    preEditTextChanged ();

    if( isTextChanged )
    {
        saveCurrentData ();
    }
    else
    {
        mainEdit.clear ();

        setWindowTitle ("NotePad - [ New ]");

        m_filePath = "";

        isTextChanged = false;
    }
}
void MainWindow::OpenFileToEditor(QString path)
{
    if( path != "")
    {
        QFile file(path);

        if( file.open (QIODevice::ReadOnly | QIODevice::Text))
        {
            mainEdit.setPlainText (QString(file.readAll ()));

            m_filePath = path;

            file.close ();

            setWindowTitle (QString("Notepad - ") + "[" + m_filePath + "]" );

            isTextChanged = false;
        }
        else
        {
            showErrorMessage (QString("open file error") + "\"" + path + "\"");
        }
    }
}

void MainWindow::onFileOpen()
{
    preEditTextChanged ();

    if( !isTextChanged )
    {
        QString path = showFileDialog(QFileDialog::AcceptOpen, "Open File");
        OpenFileToEditor (path);
    }

}


void MainWindow::onFileSave()
{
    QString path = saveCurrentData (m_filePath);

    if( path != "" )
    {
        m_filePath = path;
    }
}
void MainWindow::onFileSaveAs()
{
    QString path = saveCurrentData ();

    if( path != "" )
    {
        m_filePath = path;
    }
}

void MainWindow::onTextChanged()
{
    if( !isTextChanged )
    {
      setWindowTitle ("*" + windowTitle ());
    }
    isTextChanged = true;

//    setWindowTitle (this->windowTitle () + " * ");
}

void MainWindow::closeEvent (QCloseEvent * e)
{
    preEditTextChanged ();

    if( !isTextChanged )
    {
        QMainWindow::closeEvent (e);
    }
    else
    {
        e->ignore ();
    }
}

void MainWindow::dragEnterEvent (QDragEnterEvent* e)
{
    if( e->mimeData ()->hasUrls () )
    {
        e->acceptProposedAction ();
    }
    else
    {
        e->ignore ();
    }
}

void MainWindow::dropEvent (QDropEvent* e)
{
    if( e->mimeData ()->hasUrls () )
    {
        QList<QUrl> list = e->mimeData ()->urls ();
        QString path = list[0].toLocalFile ();
        QFileInfo fil(path);

        if( fil.isFile () )
        {
            preEditTextChanged ();

            if( !isTextChanged )
            {
                OpenFileToEditor (path);
            }
        }
        else
        {
            showErrorMessage ("Cannot open a folder!");
        }

    }
    else
    {
        e->ignore ();
    }
}