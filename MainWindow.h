#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QToolBar>
#include <QPlainTextEdit>
#include <QLabel>
#include <QFileDialog>

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

    MainWindow(QWidget *parent = 0);
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
private slots:
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onTextChanged();
};

#endif // MAINWINDOW_H
