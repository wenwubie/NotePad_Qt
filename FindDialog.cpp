#include "FindDialog.h"
#include <QEvent>
#include <QDebug>

FindDialog::FindDialog(QWidget *parent) : QDialog(parent, Qt::WindowCloseButtonHint | Qt::Drawer)
{
    m_findLabel.setText ("Find What: ");
    m_findBtn.setText ("Find Next");
    m_matchBox.setText ("Match Case");
    radioGrpBox.setTitle ("Direction");
    m_forwardBtn.setText ("Forward");
    m_forwardBtn.setChecked (true);
    m_backwardBtn.setText ("Backward");
    m_closeBtn.setText ("Close");

   hLayout.addWidget (&m_forwardBtn);
   hLayout.addWidget (&m_backwardBtn);
   radioGrpBox.setLayout (&hLayout);

//   qDebug () << "hLayout.Parent() : " << hLayout.parent ();
//   qDebug () << "&radioGrpBox: " << &radioGrpBox;
//   qDebug () << "m_forwardBtn.Parent() :" << m_forwardBtn.parent ();

   gLayout.setSpacing (10);
   gLayout.addWidget (&m_findLabel, 0, 0);
   gLayout.addWidget (&m_findEdit, 0, 1);
   gLayout.addWidget (&m_findBtn, 0, 2);
   gLayout.addWidget (&m_matchBox, 1, 0);
   gLayout.addWidget (&radioGrpBox, 1, 1);
   gLayout.addWidget (&m_closeBtn, 1, 2);

   setLayout (&gLayout);
   setWindowTitle ("Find");
}

bool FindDialog::event(QEvent* evt)
{
    if( evt->type () == QEvent::Close )
    {
        hide ();

        return true;
    }

    return QDialog::event (evt);
}
