#include "MainWindow.h"
#include "AppConfig.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QIcon>
#include <QSize>
#include <QStatusBar>
#include <QLabel>
#include <QPalette>
#include <QDebug>

MainWindow::MainWindow() :
    spFindDlg(new FindDialog(this, &mainEdit)),
    repDlg(new ReplaceDialog(this, &mainEdit))
{
    setWindowTitle ("NotePad - [ New ]");
    setAcceptDrops (true);

    isTextChanged = false;

    this->resize (800, 600);
}

MainWindow* MainWindow::newInstance()
{
    MainWindow* ret = new MainWindow();

    if( !(ret && ret->construction ()) )
    {
        delete ret;
        ret = NULL;
    }

    return ret;
}

bool MainWindow::construction()
{
    bool ret = true;

    AppConfig config;

    ret = ret && initMenuBar ();
    ret = ret && initToolBar();
    ret = ret && initStatusBar ();
    ret = ret && initMainEdit();

    if( config.isValid () )
    {
        mainEdit.setFont (config.mainEditFont ());

        if( !config.isAutoWrap () )
        {
            mainEdit.setLineWrapMode (QPlainTextEdit::NoWrap);
            findToolBarAction ("Auto Wrap")->setChecked (false);
            findMenuBarAction ("Auto Wrap")->setChecked (false);
        }

        if( !config.isStatusBarVisible () )
        {
            statusBar ()->setVisible (false);
            findToolBarAction ("Status Bar")->setChecked (false);
            findMenuBarAction ("Status Bar")->setChecked (false);
        }

        if( !config.isToolBarVisible () )
        {
            toolBar ()->setVisible (false);
            findToolBarAction ("Tool Bar")->setChecked (false);
            findMenuBarAction ("Tool Bar")->setChecked (false);
        }
    }
    return ret;
}

bool MainWindow::initMenuBar()
{
    bool ret = true;
    QMenuBar* mb = menuBar ();

    ret = ret && initFileMenu (mb);
    ret = ret && initEditMenu (mb);
    ret = ret && initFormatMenu (mb);
    ret = ret && initViewMenu (mb);
    ret = ret && initHelpMenu (mb);

    return ret;
}

bool MainWindow::initToolBar()
{
    bool ret = true;

    QToolBar* tb = addToolBar ("Tool Bar");

//    QToolBar* ftb = addToolBar ("File Tool Bar");
//    QToolBar* etb = addToolBar ("Edit Tool Bar");
//    QToolBar* fttb = addToolBar ("Format Tool Bar");
//    QToolBar* vtb = addToolBar ("View Tool Bar");

    tb->setIconSize (QSize(16, 16));

//    ftb->setIconSize (QSize(16, 16));
//    etb->setIconSize (QSize(16, 16));
//    fttb->setIconSize (QSize(16, 16));
//    vtb->setIconSize (QSize(16, 16));

    ret = ret && initToolItem (tb);
//    ret = ret && initFileToolItem (ftb);
//    ret = ret && initEditToolItem (etb);
//    ret = ret && initFormatToolItem (fttb);
//    ret = ret && initViewToolItem (vtb);

    return ret;
}

bool MainWindow::initStatusBar()
{
    bool ret = true;

    QStatusBar* sb = statusBar ();
    QLabel* label = new QLabel("Danfinswe");



    if( sb != NULL && label !=NULL )
    {
        statusLb1.setMinimumWidth (200);
        statusLb1.setAlignment (Qt::AlignCenter);
        statusLb1.setText ("Ln: 1    Col: 1");
        label->setMinimumWidth (200);
        label->setAlignment (Qt::AlignHCenter);

//        sb->addPermanentWidget (new QLabel());
        sb->addPermanentWidget (&statusLb1);
        sb->addPermanentWidget (label);

    }
    else
    {
        ret = false;
        delete label;
    }

    return ret;
}

bool MainWindow::initMainEdit()
{
    bool ret = true;
    QPalette mPalette = mainEdit.palette ();
    mainEdit.setParent (this);

    mPalette.setColor (QPalette::Inactive, QPalette::Highlight, mPalette.color (QPalette::Active, QPalette::Highlight));
    mPalette.setColor (QPalette::Inactive, QPalette::HighlightedText, mPalette.color (QPalette::Active, QPalette::HighlightedText));

    mainEdit.setPalette (mPalette);

    setCentralWidget (&mainEdit);

    connect(&mainEdit,SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    connect (&mainEdit, SIGNAL(copyAvailable(bool)), this, SLOT(onCopyAvalilable(bool)));
    connect (&mainEdit, SIGNAL(redoAvailable(bool)), this, SLOT(onRedoAvailable(bool)));
    connect (&mainEdit, SIGNAL(undoAvailable(bool)), this, SLOT(onUndoAvailable(bool)));
    connect (&mainEdit, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPosChanged()));

    return ret;
}

bool MainWindow::initToolItem(QToolBar* tb)
{
    bool ret = true;

    QAction* action = NULL;

    // new tool action
    ret = ret&& makeAction (action, tb, "New", ":/Res/pic/new.png");

    if( ret )
    {
        connect (action, SIGNAL(triggered()), this, SLOT(onFileNew()));
        tb->addAction (action);
    }

    // open tool action
    ret = ret&& makeAction (action,  tb,"Open", ":/Res/pic/open.png");

    if( ret )
    {
        connect (action, SIGNAL(triggered()), this, SLOT(onFileOpen()));
        tb->addAction (action);
    }

    // save tool action
    ret = ret&& makeAction (action,  tb, "Save", ":/Res/pic/save.png");

    if( ret )
    {
        connect (action, SIGNAL(triggered()), this, SLOT(onFileSave()));
        tb->addAction (action);
    }

    // save as tool action
    ret = ret&& makeAction (action,  tb, "Save As", ":/Res/pic/saveas.png");

    if( ret )
    {
        connect (action, SIGNAL(triggered()), this, SLOT(onFileSaveAs()));
        tb->addAction (action);
    }

    tb->addSeparator ();

    // undo action
    ret = ret&& makeAction (action,  tb, "Undo", ":/Res/pic/undo.png");

    if( ret )
    {
        connect (action, SIGNAL(triggered()), &mainEdit, SLOT(undo()));
        tb->addAction (action);
        action->setEnabled (false);
    }

    // redo action
    ret = ret&& makeAction (action,  tb, "Redo", ":/Res/pic/redo.png");

    if( ret )
    {
        connect (action, SIGNAL(triggered()), &mainEdit, SLOT(redo()));
        tb->addAction (action);
        action->setEnabled (false);
    }

    // cut action
    ret = ret&& makeAction (action, tb, "Cut", ":/Res/pic/cut.png");

    if( ret )
    {
        connect (action, SIGNAL(triggered()), &mainEdit, SLOT(cut()));
        tb->addAction (action);
        action->setEnabled (false);
    }

    // copy action
    ret = ret&& makeAction (action, tb, "Copy", ":/Res/pic/copy.png");

    if( ret )
    {
        connect (action, SIGNAL(triggered()), &mainEdit, SLOT(copy()));
        tb->addAction (action);
        action->setEnabled (false);
    }

    // paste action
    ret = ret&& makeAction (action, tb, "Paste", ":/Res/pic/paste.png");

    if( ret )
    {
        connect (action, SIGNAL(triggered()), &mainEdit, SLOT(paste()));
        tb->addAction (action);
    }

    // find action
    ret = ret&& makeAction (action, tb, "Find", ":/Res/pic/find.png");

    if( ret )
    {
        tb->addAction (action);
    }

    // replace action
    ret = ret&& makeAction (action, tb, "Replace", ":/Res/pic/replace.png");

    if( ret )
    {
        tb->addAction (action);
    }

    // goto action
    ret = ret&& makeAction (action, tb, "Goto", ":/Res/pic/goto.png");

    if( ret )
    {
        tb->addAction (action);
    }

    tb->addSeparator ();

    // print tool action
    ret = ret&& makeAction (action,  tb, "Print", ":/Res/pic/print.png");

    if( ret )
    {
    connect (action, SIGNAL(triggered()), this, SLOT(onFilePrint()));
    tb->addAction (action);
    }

    // wrap action
    ret = ret&& makeAction (action, tb, "Auto Wrap", ":/Res/pic/wrap.png");

    if( ret )
    {
        action->setCheckable (true);
        action->setChecked (true);
        connect (action, SIGNAL(triggered()), this, SLOT(onFormatWrap()));
        tb->addAction (action);
    }

    // font action
    ret = ret&& makeAction (action, tb, "Font", ":/Res/pic/font.png");

    if( ret )
    {
        connect (action, SIGNAL(triggered()), this, SLOT(onFormatFont()));
        tb->addAction (action);
    }

    tb->addSeparator ();

    // tool action
    ret = ret&& makeAction (action, tb, "Tool Bar", ":/Res/pic/tool.png");

    if( ret )
    {
        action->setCheckable (true);
        action->setChecked (true);
        connect (action, SIGNAL(triggered()), this, SLOT(onToolBar()));
       tb->addAction (action);
    }

    // status action
    ret = ret&& makeAction (action, tb, "Status Bar", ":/Res/pic/status.png");

    if( ret )
    {
        action->setCheckable (true);
        action->setChecked (true);
        connect (action, SIGNAL(triggered()), this, SLOT(onStatusBar()));
       tb->addAction (action);
    }

    return ret;
}

bool MainWindow::initFileMenu(QMenuBar* mb)
{
    QMenu* fileMenu = new QMenu("File(&F)", mb);
    bool ret = (fileMenu != NULL);

    if ( ret )
    {
        QAction* action = NULL;

        // new action
        ret = ret && makeAction (action, mb, "New(&N)", Qt::CTRL + Qt::Key_N, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), this, SLOT(onFileNew()));
            fileMenu->addAction (action);
        }

        // open action
        ret = ret && makeAction (action, mb, "Open(&O)...", Qt::CTRL + Qt::Key_O, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), this, SLOT(onFileOpen()));
            fileMenu->addAction (action);
        }

        // save action
        ret = ret && makeAction (action, mb, "Save(&S)", Qt::CTRL + Qt::Key_S, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), this, SLOT(onFileSave()));
            fileMenu->addAction (action);
        }

        // save as action
        ret = ret && makeAction (action, mb, "Save As(&A)...", 0, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), this, SLOT(onFileSaveAs()));
            fileMenu->addAction (action);
        }

        fileMenu->addSeparator ();



        // page setting action
        ret = ret && makeAction (action, mb, "Page Setting(&U)...", 0, false);

        if( ret )
        {
            fileMenu->addAction (action);
        }

        // print action
        ret = ret && makeAction (action, mb, "Print(&P)...", Qt::CTRL + Qt::Key_P, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), this, SLOT(onFilePrint()));
            fileMenu->addAction (action);
        }

        fileMenu->addSeparator ();

        // exit action
        ret = ret && makeAction (action, mb, "Exit(&X)", 0, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), this, SLOT(onExit()));
            fileMenu->addAction (action);
        }
    }

    if( ret )
    {
        mb->addMenu (fileMenu);
    }
    else
    {
        delete fileMenu;
    }


    return ret;
}

bool MainWindow::initEditMenu(QMenuBar* mb)
{
    QMenu* editMenu = new QMenu("Edit(&E)", mb);
    bool ret = (editMenu != NULL);

    if( ret )
    {
        QAction* action = NULL;

        // redo action
        ret = ret && makeAction (action, mb, "Undo(&U)", Qt::CTRL + Qt::Key_U, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), &mainEdit, SLOT(undo()));
            editMenu->addAction (action);
            action->setEnabled (false);
        }

        // redo action
        ret = ret && makeAction (action, mb, "Redo(&U)", Qt::CTRL + Qt::Key_Z, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), &mainEdit, SLOT(redo()));
            editMenu->addAction (action);
            action->setEnabled (false);
        }

        editMenu->addSeparator ();

        // cut action
        ret = ret && makeAction (action, mb, "Cut(&T)", Qt::CTRL + Qt::Key_X, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), &mainEdit, SLOT(cut()));
            editMenu->addAction (action);
            action->setEnabled (false);
        }

        // copy action
        ret = ret && makeAction (action, mb, "Copy(&C)", Qt::CTRL + Qt::Key_C, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), &mainEdit, SLOT(copy()));
            editMenu->addAction (action);
            action->setEnabled (false);
        }

        // paste action
        ret = ret && makeAction (action, mb, "Paste(&P)", Qt::CTRL + Qt::Key_V, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), &mainEdit, SLOT(paste()));
            editMenu->addAction (action);
        }

        // delete action
        ret = ret && makeAction (action, mb, "Delete(&L)", Qt::Key_Delete, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), this, SLOT(onEditDelete()));
            editMenu->addAction (action);
        }

        editMenu->addSeparator ();

        // find action
        ret = ret && makeAction (action, mb, "Find(&F)...", Qt::CTRL + Qt::Key_F, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), this, SLOT(onEditFind()));
            editMenu->addAction (action);
        }

        // find next action
        ret = ret && makeAction (action, mb, "Find Next(&N)", Qt::Key_F3, false);

        if( ret )
        {
            editMenu->addAction (action);
        }

        // replace action
        ret = ret && makeAction (action, mb, "Replace(&R)...", Qt::CTRL + Qt::Key_H, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), this, SLOT(onReplace()));
            editMenu->addAction (action);
        }

        // goto action
        ret = ret && makeAction (action, mb, "Goto(&G)...", Qt::CTRL + Qt::Key_G, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), this, SLOT(onEditGoto()));
            editMenu->addAction (action);
        }

        editMenu->addSeparator ();

        // Select All action
        ret = ret && makeAction (action, mb, "Select All(&A)", Qt::CTRL + Qt::Key_A, false);

        if( ret )
        {
            editMenu->addAction (action);
        }

        // date & time action
        ret = ret && makeAction (action, mb, "Date & Time(&D)", Qt::Key_F5, false);

        if( ret )
        {
            editMenu->addAction (action);
        }
    }

    if( ret )
    {
        mb->addMenu (editMenu);
    }
    else
    {
        delete editMenu;
    }

    return ret;
}

bool MainWindow::initFormatMenu(QMenuBar* mb)
{
     QMenu* formatMenu = new QMenu("Format(&O)", mb);
    bool ret = (formatMenu != NULL);

    if( ret )
    {
        QAction* action = NULL;

        // wrap text  action
        ret = ret && makeAction (action, mb, "Auto Wrap(&W)", 0, true);

        if( ret )
        {
            action->setCheckable (true);
            action->setChecked (true);
            connect (action, SIGNAL(triggered()), this, SLOT(onFormatWrap()));
            formatMenu->addAction (action);
        }

        // font  action
        ret = ret && makeAction (action, mb, "Font(&F)...", 0, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), this, SLOT(onFormatFont()));
            formatMenu->addAction (action);
        }
    }
    if( ret )
    {
        mb->addMenu (formatMenu);
    }
    else
    {
        delete formatMenu;
    }

    return ret;
}

bool MainWindow::initViewMenu(QMenuBar* mb)
{
    QMenu* viewMenu = new QMenu("View(&V)", mb);
    bool ret = (viewMenu != NULL);

    if( ret )
    {
        QAction* action = NULL;

        // tool  bar action
        ret = ret && makeAction (action, mb, "Tool Bar(&T)", Qt::CTRL + Qt::Key_T, false);

        if( ret )
        {
            action->setCheckable (true);
            action->setChecked (true);
            connect (action, SIGNAL(triggered()), this, SLOT(onToolBar()));
            viewMenu->addAction (action);
        }

        // status  bar action
        ret = ret && makeAction (action, mb, "Status Bar(&S)", 0, true);

        if( ret )
        {
            action->setCheckable (true);
            action->setChecked (true);
            connect (action, SIGNAL(triggered()), this, SLOT(onStatusBar()));
            viewMenu->addAction (action);
        }
    }
    if( ret )
    {
        mb->addMenu (viewMenu);
    }
    else
    {
        delete viewMenu;
    }

    return ret;
}

bool MainWindow::initHelpMenu(QMenuBar* mb)
{
    QMenu* helpMenu = new QMenu("Help(&H)", mb);
    bool ret = (helpMenu != NULL);

    if( ret )
    {
        QAction* action = NULL;

        // help action
        ret = ret && makeAction (action, mb, "Help(&H)", 0, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), this, SLOT(onHelp()));
            helpMenu->addAction (action);
        }

        // about action
        ret = ret && makeAction (action, mb, "About(&A)", 0, false);

        if( ret )
        {
            connect (action, SIGNAL(triggered()), this, SLOT(onAbout()));
            helpMenu->addAction (action);
        }
    }

    if( ret )
    {
        mb->addMenu (helpMenu);
    }
    else
    {
        delete helpMenu;
    }

    return ret;
}

bool MainWindow::makeAction(QAction*& action, QWidget* parent, QString text, int key, bool checkable)
{
    bool ret = true;
    action = new QAction(text, parent);

    if ( action != NULL )
    {
        action->setShortcut (QKeySequence(key));
        action->setCheckable (checkable);
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool MainWindow::makeAction(QAction*& action, QWidget* parent, QString tip, QString icon)
{
    bool ret = true;
    action = new QAction("", parent);

    if ( action != NULL )
    {
        action->setToolTip (tip);
        action->setIcon (QIcon(icon));
    }
    else
    {
        ret = false;
    }

    return ret;
}

QToolBar* MainWindow::toolBar()
{
    QToolBar* ret = NULL;

    const QObjectList& list = children ();

    for(int i=0; i<list.count (); i++)
    {
        QToolBar* tb = dynamic_cast<QToolBar*>(list[i]);

        if( tb != NULL )
        {
            ret = tb;
            break;
        }
    }
    return ret;
}

MainWindow::~MainWindow()
{
    QFont font = mainEdit.font ();
    bool isAutoWrap = (mainEdit.lineWrapMode () == QPlainTextEdit::WidgetWidth);
    bool sbVisible = findToolBarAction ("Status Bar")->isChecked () && findMenuBarAction ("Status Bar")->isChecked ();
    bool tbVisible = findToolBarAction ("Tool Bar")->isChecked () && findMenuBarAction ("Tool Bar")->isChecked ();
    AppConfig config(font, tbVisible, sbVisible, isAutoWrap);
    config.Store ();
}
