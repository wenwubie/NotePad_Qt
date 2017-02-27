#include "FindDialog.h"
#include <QEvent>
#include <QDebug>
#include <QTextCursor>
#include <QMessageBox>

FindDialog::FindDialog(QWidget *parent, QPlainTextEdit* pTextEdit) : QDialog(parent, Qt::WindowCloseButtonHint | Qt::Drawer)
{
    initControl ();
    connectSlots ();
   setLayout (&gLayout);
   setWindowTitle ("Find");

   setTextEdit (pTextEdit);
}

void FindDialog::initControl()
{
    m_findLabel.setText ("Find What: ");
    m_findBtn.setText ("Find Next");
    m_matchBox.setText ("Match Case");
    radioGrpBox.setTitle ("Direction");
    m_forwardBtn.setText ("Forward");
    m_backwardBtn.setText ("Backward");
    m_backwardBtn.setChecked (true);
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
}

void FindDialog::connectSlots()
{
    connect (&m_findBtn, SIGNAL(clicked()), this, SLOT(onFindBtnClicked()));
    connect(&m_closeBtn, SIGNAL(clicked()), this, SLOT(onCloseBtnClicked()));
}

void FindDialog::setTextEdit(QPlainTextEdit* textEdit)
{
    m_pTextEdit = textEdit;
}

QPlainTextEdit* FindDialog::getTextEdit(void)
{
    return m_pTextEdit;
}

void FindDialog::onFindBtnClicked()
{
    QString target = m_findEdit.text ();
    if( ( m_pTextEdit != NULL) && ( target != "") )
    {
        QString text = m_pTextEdit->toPlainText ();
        QTextCursor c = m_pTextEdit->textCursor ();
        int index = -1;

        if( m_backwardBtn.isChecked () )
        {
            index = text.indexOf (target, c.position (), m_matchBox.isChecked () ? Qt::CaseSensitive : Qt::CaseInsensitive);

            if( index >= 0 )
            {
                c.setPosition (index);
                c.setPosition (index + target.length (), QTextCursor::KeepAnchor);

                m_pTextEdit->setTextCursor (c);
            }

        }

        if( m_forwardBtn.isChecked () )
        {
            index = text.lastIndexOf (target, c.position () - text.length () - 1, m_matchBox.isChecked () ? Qt::CaseSensitive : Qt::CaseInsensitive);

            if( index >= 0 )
            {
                c.setPosition (target.length () + index);
                c.setPosition (index, QTextCursor::KeepAnchor);

                m_pTextEdit->setTextCursor (c);
            }
        }


        if( index < 0 )
        {
            QMessageBox msg;
            msg.information (this, "Find Message", "Cannot find \""+target +"\"" + "any more...",QMessageBox::Ok );
        }
    }
}

void FindDialog::onCloseBtnClicked()
{
    close();
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
