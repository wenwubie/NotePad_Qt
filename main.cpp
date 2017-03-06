#include <QtGui/QApplication>
#include <QFileInfo>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow* w = MainWindow::newInstance ();
    int ret = -1;

    if( w != NULL )
    {
        if( argc > 1)
        {
            QFileInfo fil(argv[1]);

            if( fil.exists () )
            {
                w->openFile (fil.absoluteFilePath ());
            }
        }
        w->show ();

        ret = a.exec();
    }

    delete w;

    return ret;
}
