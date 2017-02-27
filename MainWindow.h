#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QToolBar>
#include <QPlainTextEdit>
#include <QLabel>
#include <QFileDialog>
#include "FindDialog.h"
#include "ReplaceDialog.h"
#include <QSharedPointer>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    static MainWindow* newInstance();
    ~MainWindow();

private:
    QPlainTextEdit mainEdit;
    QLabel statusLb1;
    QString m_filePath;
    bool isTextChanged;
    QSharedPointer<FindDialog> spFindDlg;
    QSharedPointer<ReplaceDialog> repDlg;

    MainWindow();
    bool construction();
    bool initMenuBar();
    bool initToolBar();
    bool initStatusBar();
    bool initMainEdit();

    bool initFileMenu(QMenuBar* mb);
    bool initEditMenu(QMenuBar* mb);
    bool initFormatMenu(QMenuBar* mb);
    bool initViewMenu(QMenuBar* mb);
    bool initHelpMenu(QMenuBar* mb);

    bool initFileToolItem(QToolBar* tb);
    bool initEditToolItem(QToolBar* tb);
    bool initFormatToolItem(QToolBar* tb);
    bool initViewToolItem(QToolBar* tb);

    bool makeAction(QAction*& action, QWidget* parent, QString text, int key, bool checkable);
    bool makeAction(QAction*& action, QWidget* parent, QString tip, QString icon);

    QString showFileDialog(QFileDialog::AcceptMode mode, QString title);
    void showErrorMessage(QString msg);
    int showQueryMessage(QString msg);
    QString saveCurrentData(QString path = "");
    void preEditTextChanged();
    void OpenFileToEditor(QString path);
protected:
    void closeEvent (QCloseEvent * e);
    void dragEnterEvent (QDragEnterEvent* e);
    void dropEvent (QDropEvent* e);
    QAction* findMenuBarAction(QString text);
    QAction* findToolBarAction(QString text);

private slots:
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onTextChanged();
    void onFilePrint();
    void onCursorPosChanged();
    void onRedoAvailable(bool available);
    void onCopyAvalilable(bool available);
    void onUndoAvailable(bool available);
    void onEditDelete();
    void onEditFind();
    void onReplace();
    void onExit();

};

#endif // MAINWINDOW_H
