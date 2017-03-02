#include "MainWindow.h"
#include "AboutDialog.h"
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
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QObjectList>
#include <QUrl>
#include <QPrintDialog>
#include <QInputDialog>
#include <QKeyEvent>
#include <QFontDialog>
#include <QApplication>
#include <QDesktopServices>

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

QAction* MainWindow::findMenuBarAction(QString text)
{
    QAction* ret = NULL;
    const QObjectList& list = menuBar ()->children ();

    for(int i=0; i<list.count (); i++)
    {
        QMenu* menu = dynamic_cast<QMenu*>(list[i]);

        if( menu != NULL)
        {
            QList<QAction*> actionList = menu->actions ();
            for(int j=0; j<actionList.count (); j++)
            {
                if( actionList[j]->text ().startsWith (text) )
                {
                    ret = actionList[j];
                    break;
                }
            }
        }
    }
    return ret;
}

QAction* MainWindow::findToolBarAction(QString text)
{
    QAction* ret = NULL;
    QToolBar* tb = toolBar ();
    if ( tb != NULL )
    {
        QList<QAction*> actionList = tb->actions ();
        for(int j=0; j<actionList.count (); j++)
        {
            if( actionList[j]->toolTip ().startsWith (text) )
            {
                ret = actionList[j];
                break;
            }
        }
    }
    return ret;
}

void MainWindow::onRedoAvailable(bool available)
{
    findMenuBarAction ("Redo")->setEnabled (available);
    findToolBarAction ("Redo")->setEnabled (available);
}

void MainWindow::onCopyAvalilable(bool available)
{
    findMenuBarAction ("Copy")->setEnabled (available);
    findToolBarAction ("Copy")->setEnabled (available);
    findMenuBarAction ("Cut")->setEnabled (available);
    findToolBarAction ("Cut")->setEnabled (available);
}

void MainWindow::onUndoAvailable(bool available)
{
    findMenuBarAction ("Undo")->setEnabled (available);
    findToolBarAction ("Undo")->setEnabled (available);
}

void MainWindow::onFilePrint()
{
    QPrintDialog dlg(this);

    dlg.setWindowTitle ("Print");

    if( dlg.exec () == QPrintDialog::Accepted )
    {
        QPrinter* p = dlg.printer ();

        mainEdit.document ()->print (p);
    }
}

void MainWindow::onCursorPosChanged()
{
    int pos = mainEdit.textCursor ().position ();
    QString text = mainEdit.toPlainText ();
    int ln = 0;
    int col = 0;
    int flag = -1;

    for(int i=0; i<pos; i++)
    {
        if( text[i] == '\n' )
        {
            ln++;
            flag = i;
        }
    }

    flag++;

    col = pos - flag;

    statusLb1.setText ("Ln: " + QString::number (ln + 1) + "   Col: " + QString::number (col + 1));
}

void MainWindow::onEditDelete()
{
    QKeyEvent keyPress(QKeyEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier);
    QKeyEvent keyRelease(QKeyEvent::KeyRelease, Qt::Key_Delete, Qt::NoModifier);

    QApplication::sendEvent (&mainEdit, &keyPress);
    QApplication::sendEvent (&mainEdit, &keyRelease);
}

void MainWindow::onEditFind()
{
    spFindDlg->show ();
}

void MainWindow::onReplace()
{
    repDlg->show ();
}

void MainWindow::onEditGoto()
{

    bool ok = false;
    int ln = QInputDialog::getInt (this, "Goto", "Line:", 1, 1, mainEdit.document ()->lineCount (), 1, &ok, Qt::WindowCloseButtonHint | Qt::Drawer);

    if( ok )
    {
        QString text = mainEdit.toPlainText ();
        QTextCursor c = mainEdit.textCursor ();
        int pos = 0;
        int next = -1;

        for(int i=0; i<ln; i++)
        {
                pos = next + 1;
                next = text.indexOf ('\n', pos);
        }

        c.setPosition (pos);

        mainEdit.setTextCursor (c);
    }
}

void MainWindow::onStatusBar()
{
    QStatusBar* sb = statusBar ();
    bool visible = sb->isVisible ();

    sb->setVisible (!visible);

    findToolBarAction ("Status Bar")->setChecked (!visible);
    findMenuBarAction ("Status Bar")->setChecked (!visible);

}

void MainWindow::onToolBar()
{
    QToolBar* tb = toolBar ();

    if( tb != NULL )
    {
        bool visible = tb->isVisible ();

        tb->setVisible (!visible);

        findToolBarAction ("Tool Bar")->setChecked(!visible);
        findMenuBarAction ("Tool Bar")->setChecked (!visible);
    }

}

void MainWindow::onFormatFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont (&ok, mainEdit.font (), this);

    if( ok )
    {
        mainEdit.setFont (font);
    }
}

void MainWindow::onFormatWrap()
{
    QPlainTextEdit::LineWrapMode mode = mainEdit.lineWrapMode ();

    if( mode == QPlainTextEdit::NoWrap )
    {
        mainEdit.setLineWrapMode (QPlainTextEdit::WidgetWidth);

        findMenuBarAction ("Auto Wrap")->setChecked (true);
        findToolBarAction ("Auto Wrap")->setChecked (true);
    }
    else
    {
        mainEdit.setLineWrapMode (QPlainTextEdit::NoWrap);

        findMenuBarAction ("Auto Wrap")->setChecked (false);
        findToolBarAction ("Auto Wrap")->setChecked (false);
    }
}

void MainWindow::onExit()
{
    close();
}

void MainWindow::onHelp()
{
//    QDesktopServices::openUrl(QUrl("file:///F:/program/Project/NotePad/help.txt"));
    QDesktopServices::openUrl(QUrl("http://www.baidu.com"));
}

void MainWindow::onAbout()
{
    AboutDialog(this).exec ();
}
